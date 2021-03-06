/*

    Interface definitions for bget.c, the memory management package.

*/
typedef long bufsize;

struct qlinks {
	struct bfhead *flink;	      /* Forward link */
	struct bfhead *blink;	      /* Backward link */
};

/* Header in allocated and free buffers */

struct bhead {
	bufsize prevfree;		      /* Relative link back to previous
														free buffer in memory or 0 if
														previous buffer is allocated.	*/
	bufsize bsize;		      /* Buffer size: positive if free,
													negative if allocated. */
};
#define BH(p)	((struct bhead *) (p))

/*  Header in directly allocated buffers (by acqfcn) */

struct bdhead {
	bufsize tsize;		      /* Total size, including overhead */
	struct bhead bh;		      /* Common header */
};
#define BDH(p)	((struct bdhead *) (p))

/* Header in free buffers */

struct bfhead {
	struct bhead bh;		      /* Common allocated/free header */
	struct qlinks ql;		      /* Links on free list */
};
#define BFH(p)	((struct bfhead *) (p))


class BGet
{
public:
	BGet();
	struct bfhead freelist;/* = {
												 {0, 0},
												 {&freelist, &freelist}
	};*/     /* List of free buffers */

	void *(*acqfcn) (bufsize size);;

	void	bpool	    (void *buffer, bufsize len);
	void   *bget	    (bufsize size);
	void   *bgetz	    (bufsize size);
	void   *bgetr	    (void *buffer, bufsize newsize);
	void	brel	    (void *buf);
	void	bectl	    (void *(*acquire) (bufsize size), bufsize pool_incr);
};
