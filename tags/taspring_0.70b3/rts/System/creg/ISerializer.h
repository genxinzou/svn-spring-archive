
#ifndef SERIALIZE_IFACE_H
#define SERIALIZE_IFACE_H

namespace creg
{
	class Class;

	/// Serializer interface
	class ISerializer
	{
	public:
		virtual bool IsWriting () = 0;

		/// Serialize a memory buffer
		virtual void Serialize (void *data, int byteSize) = 0;

		/// Serialize a pointer to an instance of a creg registered class/struct
		virtual void SerializeObjectPtr (void **ptr, Class *objectClass) = 0;
		
		/** Serialize an instance of an object, could be a structure embedded into another object
		 * The big difference with SerializeObjectPtr, is that the caller of this function
		 * controls the allocation of the object instead of the creg serializer itself */
		virtual void SerializeObjectInstance (void *inst, Class *objectClass) = 0;
	};

};

#endif
