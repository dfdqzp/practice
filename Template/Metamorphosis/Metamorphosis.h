// Reflactor NVI implement in <Effective c++>
// author : hanmz
// Date : 2013-12-20
template <class T, class FP>
class Metamorphosis : public T
{
public:
	FP CallPrivateOfBase(int idx)
	{
		return (FP)*((int*)*(int*)(this) + idx);
	}
};
