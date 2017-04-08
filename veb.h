
unsigned long long const NO = -1;

template<unsigned int S>
class AbstractVEBTree {
public:
  virtual void add(unsigned long long x) = 0;
  virtual void remove(unsigned long long x) = 0;
  virtual unsigned long long next(unsigned long long x) const = 0;
  virtual unsigned long long prev(unsigned long long x) const = 0;
  virtual unsigned long long getMin() const = 0;
  virtual unsigned long long getMax() const = 0;
};

template<unsigned int S>
class VEBTree;
