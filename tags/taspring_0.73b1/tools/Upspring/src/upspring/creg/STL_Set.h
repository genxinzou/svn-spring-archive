#ifndef CR_SET_TYPE_IMPL_H
#define CR_SET_TYPE_IMPL_H

#include <set>

namespace creg 
{
	// set/multiset - this template assumes the type is copyable
	template<typename T>
	class SetType : public IType
	{
		IType *elemType;
	public:
		typedef typename T::iterator iterator;

		SetType (IType *elemType) :  elemType (elemType) {}
		~SetType () { delete elemType; }

		void Serialize (ISerializer *s, void *instance)
		{
			T& ct = *(T*)instance;
			if (s->IsWriting ()) {
				int size=ct.size();
				s->Serialize (&size, sizeof(int));
				for (iterator i=ct.begin();i!=ct.end();++i) 
					elemType->Serialize (s, &*i);
			} else {
				int size;
				s->Serialize (&size, sizeof(int));
				for (int i=0;i<size;i++) {
					typename T::value_type v;
					elemType->Serialize (s, &v);
					ct.insert (v);
				}
			}
		}
	};


	// Set type
	template<typename T>
	struct DeduceType < std::set<T> > {
		IType* Get () {
			DeduceType<T> elemtype;
			return new SetType < std::set <T> > (elemtype.Get());
		}
	};
	// Multiset
	template<typename T>
	struct DeduceType < std::multiset<T> > {
		IType* Get () {
			DeduceType<T> elemtype;
			return new SetType < std::multiset<T> > (elemtype.Get());
		}
	};
};
#endif

