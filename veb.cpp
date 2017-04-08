#include <unordered_map>
#include "veb.h"

using namespace std;

#define veb_tree VEBTree
#define min getMin
#define max getMax


template <unsigned int S>
struct veb_tree: AbstractVEBTree<S>
{
	typedef unsigned long long type;
	
	veb_tree():
		mn(NO),
		mx(NO),
		_size(0),
		help(new veb_tree <S / 2>())
	{}

	virtual ~veb_tree()
	{
		delete help;
		for (auto it: clusters)
		{
			delete it.second;
		}
	}

	size_t size() const
	{
		return _size;
	}

	type next(type a) const
	{
		if (a >= mx)
		{
			return NO;
		}
		
		if (a < mn)
		{
			return mn;
		}
		create_if_disappear(high(a));
		if (clusters[high(a)]->size() == 0 || clusters[high(a)]->max() <= low(a))
		{
			type now = help->next(high(a));
			if (now == NO)
				return NO;
			return collect(now, clusters[now]->min());
		}
		else
		{
			type now = clusters[high(a)]->next(low(a));
			return collect(high(a), now);
		}
	}

	type prev(type a) const
	{
		if (a <= mn)
		{
			return NO;
		}

		if ((size() == 1 && mn < a) || (help->min() > high(a) || (help->min() == high(a) && clusters[high(a)]->min() >= low(a))))
		{
			return mn;
		}
		create_if_disappear(high(a));
		if (clusters[high(a)]->size() == 0 || clusters[high(a)]->min() >= low(a))
		{
			type now = help->prev(high(a));
			if (now == NO)
				return NO;
			return collect(now, clusters[now]->max());
		}
		else
		{
			type now = clusters[high(a)]->prev(low(a));
			return collect(high(a), now);
		}
	}

	void insert(type a)
	{
		_size++;
		if (size() == 1)
		{
			mn = a;
			mx = a;
			return;
		}
		if (a < mn)
		{
			swap(a, mn);
		}
		if (mx < a)
		{
			mx = a;
		}
		create_if_disappear(high(a));
		if (clusters[high(a)]->size() == 0)
		{
			help->insert(high(a));
		}
		clusters[high(a)]->insert(low(a));
	}

	bool contains(type a) const
	{
		if (mn == a)
		{
			return true;
		}
		return clusters.find(high(a)) != clusters.end() &&
			clusters[high(a)]->contains(low(a));
			
	}

	void erase(type a)
	{
		_size--;
		if (size() == 0)
		{
			mx = NO;
			mn = NO;
			return;
		}
		type now;
		if (a == mn)
		{
			now = help->min();
			mn = collect(now, clusters[now]->min());
			clusters[now]->erase(low(mn));
		}
		else
		{
			now = high(a);
			clusters[high(a)]->erase(low(a));
		}
		if (clusters[now]->size() == 0)
		{
			help->erase(now);
		}
		if (a == mx)
		{
			if (size() > 1)
			{
				type now = help->max();
				mx = collect(now, clusters[now]->max());
			}
			else if (size() == 1)
			{
				mx = mn;
			}
		}
	}

	type min() const
	{
		return mn;
	}

	type max() const
	{
		return mx;
	}
	
	void add(type a)
	{
		if (!contains(a))
		{
			insert(a);
		}
	}

	void remove(type a)
	{
		if (contains(a))
		{
			erase(a);
		}
	}	

	private:

	void create_if_disappear(type a) const
	{
		if (clusters.find(a) == clusters.end())
		{
			clusters.insert({a, new veb_tree <(S + 1) / 2>()});
		}
	}

	static type collect(type h, type l)
	{
		return (h << ((S + 1) / 2)) + l;
	}
	
	static type high(type a)
	{
		return a >> ((S + 1) / 2);
	}

	static type low(type a)
	{
		return a - (high(a) << ((S + 1) / 2));
	}

	type mn, mx;
	size_t _size;
	mutable unordered_map <type, veb_tree <(S + 1) / 2>*> clusters;
	veb_tree <S / 2>* help;
};

template <>
struct veb_tree<1>: AbstractVEBTree<1>
{
	typedef unsigned long long type;

	veb_tree():
		mn(NO),
		mx(NO),
		_size(0)
	{}

	virtual ~veb_tree()
	{}

	bool contains(type a) const
	{
		return mn == a || mx == a;
	}
	
	void insert(type a)
	{
		_size++;
		if (a == 0)
		{
			mn = a;
		}
		else
		{
			mx = a;
		}
	}

	void erase(type a)
	{
		_size--;
		if (mn == a)
		{
			mn = NO;
		}
		if (mx == a)
		{
			mx = NO;
		}
	}

	type next(type a) const
	{
		if (mx != NO && a == 0)
		{
			return mx;
		}
		return NO;
	}

	type prev(type a) const
	{
		if (mn != NO && a == 1)
		{
			return mn;
		}
		return NO;
	}
	
	size_t size() const
	{
		return _size;
	}

	type min() const
	{
		if (mn != NO)
			return mn;
		return mx;
	}

	type max() const
	{
		if (mx != NO)
			return mx;
		return mn;
	}

	void add(type a)
	{
		if (!contains(a))
			insert(a);
	}

	void remove(type a)
	{
		if (contains(a))
			erase(a);
	}

	private:
	type mn, mx;
	size_t _size;
};
