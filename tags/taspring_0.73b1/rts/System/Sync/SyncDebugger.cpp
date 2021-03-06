/* Author: Tobi Vollebregt */

#include "StdAfx.h"

#ifdef SYNCDEBUG

#include "LogOutput.h"
#include "System/GlobalStuff.h"
#include "System/Net.h"
#include "SyncDebugger.h"
#include "Logger.h"


#ifndef WIN32
/* for backtrace() function */
# include <execinfo.h>
# define HAVE_BACKTRACE
#elif defined __MINGW32__
/* from backtrace.c: */
extern "C" int backtrace (void **array, int size);
# define HAVE_BACKTRACE
#else
# undef HAVE_BACKTRACE
#endif


#define LOGFILE_SERVER   "syncdebug-server.log"
#define LOGFILE_CLIENT   "syncdebug-client.log"


// externals
extern bool globalQuit;


/**
 * @brief logging instance
 */
CLogger logger;


/**
 * @brief get sync debugger instance
 *
 * The sync debugger is a singleton (for now).
 * @return the instance
 */
CSyncDebugger* CSyncDebugger::GetInstance() {
	static CSyncDebugger instance;
	return &instance;
}


/**
 * @brief default-noarg-constructor
 */
CSyncDebugger::CSyncDebugger():
		history(NULL), historybt(NULL), historyIndex(0),
		disable_history(false), may_enable_history(false),
		flop(0), waitingForBlockResponse(false)
{
	// Need to allocate those here instead of having them inlined in the class,
	// because of #include dependency: MAX_PLAYERS is in GlobalStuff.h, which
	// needs creg.h. But creg.h needs SyncedPrimitive.h, which needs
	// SyncDebugger.h. Hence we can't use MAX_PLAYERS in SyncDebugger.h.  :-)
	checksumResponses = new std::vector<unsigned> [MAX_PLAYERS];
	remoteHistory     = new std::vector<unsigned> [MAX_PLAYERS];
	remoteFlop = new Uint64 [MAX_PLAYERS];
}


/**
 * @brief destructor
 */
CSyncDebugger::~CSyncDebugger()
{
	delete[] history;
	delete[] historybt;
	delete[] checksumResponses;
	delete[] remoteHistory;
	delete[] remoteFlop;
}


/**
 * @brief initialize
 *
 * Initialize the sync debugger. Pass true for a server (this requires approx.
 * 128 megabytes on 32 bit systems and 224 megabytes on 64 bit systems) and
 * false for a client (requires only 16 megabytes extra).
 */
void CSyncDebugger::Initialize(bool useBacktrace)
{
	delete[] history;
	history = 0;
	delete[] historybt;
	historybt = 0;

#ifdef HAVE_BACKTRACE
	if (useBacktrace) {
		historybt = new HistItemWithBacktrace[HISTORY_SIZE * BLOCK_SIZE];
		memset(historybt, 0, HISTORY_SIZE * BLOCK_SIZE * sizeof(HistItemWithBacktrace));
	} else
#endif
	{
		history = new HistItem[HISTORY_SIZE * BLOCK_SIZE];
		memset(history, 0, HISTORY_SIZE * BLOCK_SIZE * sizeof(HistItem));
	}

	//cleanup
	historyIndex = 0;
	disable_history = false;
	may_enable_history = false;
	flop = 0;
	for (int j = 0; j < MAX_PLAYERS; ++j) {
		checksumResponses[j].clear();
		remoteHistory[j].clear();
		remoteFlop[j] = 0;
	}
	pendingBlocksToRequest.clear();
	waitingForBlockResponse = false;

	// init logger
	logger.SetFilename(useBacktrace ? LOGFILE_SERVER : LOGFILE_CLIENT);
}


/**
 * @brief the backbone of the sync debugger
 *
 * This function adds an item to the history and appends a backtrace and an
 * operator (op) to it. p must point to the data to checksum and size must be
 * the size of that data.
 */
void CSyncDebugger::Sync(void* p, unsigned size, const char* op)
{
	HistItem* h = &history[historyIndex];

#ifdef HAVE_BACKTRACE
	if (historybt) {
		// dirty hack to skip the uppermost 2 frames without memcpy'ing the whole backtrace...
		// note that 2 frames is the max because of the members of HistItem and HistItemWithBacktrace
		historybt[historyIndex].bt_size = backtrace(historybt[historyIndex].bt - 2, MAX_STACK + 2) - 2;
		historybt[historyIndex].op = op;
		h = &historybt[historyIndex];
	}
#endif

	// If it crashes here you probably forgot to call Initialize().
	unsigned i = 0;
	h->chk = 0;
	for (; i < (size & ~3); i += 4)
		h->chk ^= *(unsigned*) ((unsigned char*) p + i);
	for (; i < size; ++i)
		h->chk ^= *((unsigned char*) p + i);

	if (++historyIndex == HISTORY_SIZE * BLOCK_SIZE)
		historyIndex = 0; // wrap around
	++flop;
}


/**
 * @brief output a backtrace to the log
 *
 * Writes the backtrace attached to history item # index to the log.
 * The backtrace is prefixed with prefix.
 */
void CSyncDebugger::Backtrace(int index, const char* prefix) const
{
	if (historybt) {
		for (unsigned i = 0; i < historybt[index].bt_size; ++i) {
			// the "{%p}" part is resolved to "functionname [filename:lineno]"
			// by the CLogger class.
			logger.AddLine("%s#%u {%p}", prefix, i, historybt[index].bt[i]);
		}
	}
}


/**
 * @brief get a checksum for a backtrace in the history
 *
 * @return a checksum for backtrace # index in the history.
 */
unsigned CSyncDebugger::GetBacktraceChecksum(int index) const
{
	unsigned checksum = 0;
	const unsigned* p = (const unsigned*) historybt[index].bt;
	for (unsigned i = 0; i < (sizeof(void*)/sizeof(unsigned)) * historybt[index].bt_size; ++i, ++p)
		checksum = 33 * checksum + *p;
	return checksum;
}


/**
 * @brief serverside network receiver
 *
 * Plugin for the CGameServer network code in GameServer.cpp.
 * @return the number of bytes read from the network stream
 */
int CSyncDebugger::ServerReceived(const unsigned char* inbuf)
{
	int length = 0;
	switch (inbuf[0]) {
		case NETMSG_SD_CHKRESPONSE:
			if (*(short*)&inbuf[1] != HISTORY_SIZE * sizeof(unsigned) + 12) {
				logger.AddLine("Server: received checksum response of %d instead of %d bytes", *(short*)&inbuf[1], HISTORY_SIZE * 4 + 12);
			} else {
				int player = inbuf[3];
				if(player >= gs->activeTeams || player < 0) {
					logger.AddLine("Server: got invalid playernum %d in checksum response", player);
				} else {
					const unsigned* begin = (unsigned*)&inbuf[12];
					const unsigned* end = begin + HISTORY_SIZE;
					checksumResponses[player].resize(HISTORY_SIZE);
					std::copy(begin, end, checksumResponses[player].begin());
					remoteFlop[player] = *(Uint64*)&inbuf[4];
					int i = 0;
					while (i < gs->activeTeams && !checksumResponses[i].empty()) ++i;
					if (i == gs->activeTeams) {
						ServerQueueBlockRequests();
						logger.AddLine("Server: checksum responses received; %d block requests queued", pendingBlocksToRequest.size());
					}
				}
			}
			length = *(short*)&inbuf[1];
			break;
		case NETMSG_SD_BLKRESPONSE:
			if (*(short*)&inbuf[1] != BLOCK_SIZE * sizeof(unsigned) + 4) {
				logger.AddLine("Server: received block response of %d instead of %d bytes", *(short*)&inbuf[1], BLOCK_SIZE * 4 + 4);
			} else {
				int player = inbuf[3];
				if(player >= gs->activeTeams || player < 0) {
					logger.AddLine("Server: got invalid playernum %d in block response", player);
				} else {
					const unsigned* begin = (unsigned*)&inbuf[4];
					const unsigned* end = begin + BLOCK_SIZE;
					unsigned size = remoteHistory[player].size();
					remoteHistory[player].resize(size + BLOCK_SIZE);
					std::copy(begin, end, remoteHistory[player].begin() + size);
					int i = 0;
					size += BLOCK_SIZE;
					while (i < gs->activeTeams && size == remoteHistory[i].size()) ++i;
					if (i == gs->activeTeams) {
						logger.AddLine("Server: block responses received");
						ServerReceivedBlockResponses();
					}
				}
			}
			length = *(short*)&inbuf[1];
			break;
		default:
			logger.AddLine("Server: invalid msg");
			length = 0;
			break;
	}
	return length;
}


/**
 * @brief clientside network receiver
 *
 * Plugin for the CGame network code in Game.cpp.
 * @return the number of bytes read from the network stream
 */
int CSyncDebugger::ClientReceived(const unsigned char* inbuf)
{
	int length = 0;
	switch (inbuf[0]) {
		case NETMSG_SD_CHKREQUEST:
			if (gs->frameNum != *(int*)&inbuf[1]) {
				logger.AddLine("Client: received checksum request for frame %d instead of %d", *(int*)&inbuf[1], gs->frameNum);
			} else {
				disable_history = true; // no more additions to the history until we're done
				may_enable_history = false;
				ClientSendChecksumResponse();
				logger.AddLine("Client: checksum response sent");
			}
			length = 5;
			break;
		case NETMSG_SD_BLKREQUEST:
			if (*(unsigned short*)&inbuf[1] >= HISTORY_SIZE) {
				logger.AddLine("Client: invalid block number %d in block request", *(unsigned short*)&inbuf[1]);
			} else {
				ClientSendBlockResponse(*(unsigned short*)&inbuf[1]);
				logger.AddLine("Client: block response sent for block %d", *(unsigned short*)&inbuf[1]);
				// simple progress indication
				logOutput.Print("[SD] Client: %d / %d", *(unsigned short*)&inbuf[3], *(unsigned short*)&inbuf[5]);
			}
			length = 7;
			break;
		case NETMSG_SD_RESET:
			logger.CloseSession();
			logOutput.Print("[SD] Client: Done!");
// 			disable_history = false;
			may_enable_history = true;
			if (gu->autoQuit) {
				logOutput.Print("[SD] Client: Automatical quit enforced from commandline");
				globalQuit = true;
			}
			length = 1;
			break;
		default:
			logOutput.Print("[SD] Client: invalid msg");
			length = 0;
			break;
	}
	return length;
}


/**
 * @brief first step after desync
 *
 * Called by server to trigger sync error handling.
 * It sets up some variables and sends a checksum request to all clients.
 */
void CSyncDebugger::TriggerSyncErrorHandling(int serverframenum)
{
	if (!disable_history) {
		//this will set disable_history = true once received so only one sync errors is handled at a time.
		serverNet->SendData< unsigned char, unsigned char >(NETMSG_PAUSE, true, gu->myPlayerNum);
		serverNet->SendData< int >(NETMSG_SD_CHKREQUEST, serverframenum);
	}
}


/**
 * @brief second step after desync
 *
 * Called by client to send a response to a checksum request.
 */
void CSyncDebugger::ClientSendChecksumResponse()
{
	std::vector<unsigned> checksums;
	for (unsigned i = 0; i < HISTORY_SIZE; ++i) {
		unsigned checksum = 0;
		for (unsigned j = 0; j < BLOCK_SIZE; ++j) {
			if (historybt)
				checksum = 33 * checksum + historybt[BLOCK_SIZE * i + j].chk;
			else  checksum = 33 * checksum + history[BLOCK_SIZE * i + j].chk;
		}
		checksums.push_back(checksum);
	}
	net->SendSTLData< unsigned char, Uint64, std::vector<unsigned> >(NETMSG_SD_CHKRESPONSE, gu->myPlayerNum, flop, checksums);
}


/**
 * @brief third step after desync
 *
 * Called by server after all checksum responses have been received.
 * Compares the checksumResponses and figures out which blocks are out of sync
 * (have different checksum). For these blocks requests are queued which will
 * be send next frames (one request at a time, see
 * CSyncDebugger::ServerHandlePendingBlockRequests()).
 */
void CSyncDebugger::ServerQueueBlockRequests()
{
	Uint64 correctFlop = 0;
	for (int j = 0; j < gs->activeTeams; ++j) {
		if (correctFlop) {
			if (remoteFlop[j] != correctFlop)
				logger.AddLine("Server: bad flop# %llu instead of %llu for player %d", remoteFlop[j], correctFlop, j);
		} else {
			correctFlop = remoteFlop[j];
		}
	}
	unsigned i = ((unsigned)(correctFlop % (HISTORY_SIZE * BLOCK_SIZE)) / BLOCK_SIZE) + 1, c = 0;
	for (; c < HISTORY_SIZE; ++i, ++c) {
		unsigned correctChecksum = 0;
		if (i == HISTORY_SIZE) i = 0;
		for (int j = 0; j < gs->activeTeams; ++j) {
			if (correctChecksum && checksumResponses[j][i] != correctChecksum) {
				pendingBlocksToRequest.push_back(i);
				break;
			}
			correctChecksum = checksumResponses[j][i];
		}
	}
	if (!pendingBlocksToRequest.empty()) {
		logger.AddLine("Server: blocks: %u equal, %u not equal", HISTORY_SIZE - pendingBlocksToRequest.size(), pendingBlocksToRequest.size());
		requestedBlocks = pendingBlocksToRequest;
		// we know the first FPU bug occured in block # ii, so we send out a block request for it.
// 		serverNet->SendData<unsigned> (NETMSG_SD_BLKREQUEST, ii);
	} else {
		logger.AddLine("Server: huh, all blocks equal?!?");
		serverNet->SendData(NETMSG_SD_RESET);
	}
	//cleanup
	for (int j = 0; j < MAX_PLAYERS; ++j)
		checksumResponses[j].clear();
}


/**
 * @brief helper for the third step
 *
 * Must be called by the server in GameServer.cpp once every frame to handle
 * queued block requests (see CSyncDebugger::ServerQueueBlockRequests()).
 */
void CSyncDebugger::ServerHandlePendingBlockRequests()
{
	if (!pendingBlocksToRequest.empty() && !waitingForBlockResponse) {
		// last two shorts are for progress indication
		serverNet->SendData<unsigned short, unsigned short, unsigned short> (NETMSG_SD_BLKREQUEST, pendingBlocksToRequest.front(), requestedBlocks.size() - pendingBlocksToRequest.size() + 1, requestedBlocks.size());
		waitingForBlockResponse = true;
	}
}


/**
 * @brief fourth step after desync
 *
 * Called by client to send a response to a block request.
 */
void CSyncDebugger::ClientSendBlockResponse(int block)
{
	std::vector<unsigned> checksums;
	for (unsigned i = 0; i < BLOCK_SIZE; ++i) {
		if (historybt)
			checksums.push_back(historybt[BLOCK_SIZE * block + i].chk);
		else  checksums.push_back(history[BLOCK_SIZE * block + i].chk);
	}
	net->SendSTLData< unsigned char, std::vector<unsigned> >(NETMSG_SD_BLKRESPONSE, gu->myPlayerNum, checksums);
}


/**
 * @brief fifth step after desync
 *
 * Called each time a set of blockResponses (one for every client) is received.
 * If there are no more pendingBlocksToRequest, it triggers the sixth step,
 * ServerDumpStack().
 */
void CSyncDebugger::ServerReceivedBlockResponses()
{
	pendingBlocksToRequest.pop_front();
	waitingForBlockResponse = false;
	// analyse data and reset if this was the last block response
	if (pendingBlocksToRequest.empty())
		ServerDumpStack();
}


/**
 * @brief sixth step after desync
 *
 * Called by server once all blockResponses are received. It dumps a backtrace
 * to the logger for every checksum mismatch in the block which was out of
 * sync. The backtraces are passed to the logger in a fairly simple form
 * consisting basically only of hexadecimal addresses. The logger class
 * resolves those to function, filename & line number.
 */
void CSyncDebugger::ServerDumpStack()
{
	// first calculate start iterator...
	unsigned posInHistory = (unsigned)(remoteFlop[0] % (HISTORY_SIZE * BLOCK_SIZE));
	logger.AddLine("Server: position in history: %u", posInHistory);
	unsigned blockNr = posInHistory / BLOCK_SIZE;
	unsigned virtualBlockNr = 0; // block nr in remoteHistory (which skips unchanged blocks)
	for (; virtualBlockNr < requestedBlocks.size() && requestedBlocks[virtualBlockNr] != blockNr; ++virtualBlockNr) {}
	unsigned virtualPosInHistory = (virtualBlockNr * BLOCK_SIZE) + (posInHistory % BLOCK_SIZE) + 1;
	unsigned virtualHistorySize = remoteHistory[0].size();
	if (virtualBlockNr >= requestedBlocks.size())
		virtualPosInHistory = virtualHistorySize;
	unsigned ndif = 0; // number of differences
	assert(virtualPosInHistory <= virtualHistorySize);

	// we make a pool of backtraces (to merge identical ones)
	unsigned curBacktrace = 0;
	std::map<unsigned, unsigned> checksumToIndex;
	std::map<unsigned, unsigned> indexToHistPos;

	// then loop from virtualPosInHistory to virtualHistorySize and from 0 to virtualPosInHistory.
	for (unsigned i = virtualPosInHistory, c = 0; c < virtualHistorySize; ++i, ++c) {
		unsigned correctChecksum = 0;
		if (i == virtualHistorySize) i = 0;
		bool err = false;
		for (int j = 0; j < gs->activeTeams; ++j) {
			if (correctChecksum && remoteHistory[j][i] != correctChecksum) {
				if (historybt) {
					virtualBlockNr = i / BLOCK_SIZE;
					blockNr = requestedBlocks[virtualBlockNr];
					unsigned histPos = blockNr * BLOCK_SIZE + i % BLOCK_SIZE;
					unsigned checksum = GetBacktraceChecksum(histPos);
					std::map<unsigned, unsigned>::iterator it = checksumToIndex.find(checksum);
					if (it == checksumToIndex.end()) {
						++curBacktrace;
						checksumToIndex[checksum] = curBacktrace;
						indexToHistPos[curBacktrace] = histPos;
					}
					logger.AddLine("Server: chk %08X, %15.8e instead of %08X, %15.8e, backtrace %u in \"%s\"", remoteHistory[j][i], *(float*)&remoteHistory[j][i], correctChecksum, *(float*)&correctChecksum, checksumToIndex[checksum], historybt[histPos].op);
				} else {
					logger.AddLine("Server: chk %08X, %15.8e instead of %08X, %15.8e", remoteHistory[j][i], *(float*)&remoteHistory[j][i], correctChecksum, *(float*)&correctChecksum);
				}
				err = true;
			} else {
				correctChecksum = remoteHistory[j][i];
			}
		}
		if (err) {
			++ndif;
		}
	}
	if (ndif)
		logger.AddLine("Server: chks: %d equal, %d not equal", virtualHistorySize - ndif, ndif);
	else
		// This is impossible (internal error).
		// Server first finds there are differing blocks, then all checksums equal??
		logger.AddLine("Server: huh, all checksums equal?!? (INTERNAL ERROR)");

	//cleanup
	for (int j = 0; j < MAX_PLAYERS; ++j)
		remoteHistory[j].clear();

	if (historybt) {
		// output backtraces we collected earlier this function
		for (std::map<unsigned, unsigned>::iterator it = indexToHistPos.begin(); it != indexToHistPos.end(); ++it) {
			logger.AddLine("Server: === Backtrace %u ===", it->first);
			Backtrace(it->second, "Server: ");
		}
	}

	// and reset
	serverNet->SendData(NETMSG_SD_RESET);
	logger.AddLine("Server: Done!");
	logger.CloseSession();
	logOutput.Print("[SD] Server: Done!");
}


/**
 * @brief helper network plugin function
 *
 * @return the size of the sync debugger related net message in inbuf
 * at index 0 (if any), zero otherwise.
 */
int CSyncDebugger::GetMessageLength(const unsigned char* inbuf) const
{
	switch (inbuf[0]) {
		case NETMSG_SD_CHKREQUEST:
			return 5;
		case NETMSG_SD_BLKREQUEST:
			return 7;
		case NETMSG_SD_CHKRESPONSE:
		case NETMSG_SD_BLKRESPONSE:
			return *(short*)&inbuf[1];
		case NETMSG_SD_RESET:
			return 1;
		default:
			return 0;
	}
}


/**
 * @brief re-enable the history
 *
 * Restart the sync debugger lifecycle, so it can be used again (if the sync
 * errors are resolved somehow or you were just testing it using .fakedesync).
 */
void CSyncDebugger::Reset()
{
	if (may_enable_history)
		disable_history = false;
}

#endif // SYNCDEBUG
