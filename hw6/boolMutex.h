class boolMutex
{
public:
  boolMutex(const bool initial_val) :
    flag(initial_val) {}
    
  inline bool lock() {
    __disable_irq();
    bool result(!flag);
    flag = true;
    __enable_irq();
    return result;
  }
  
  inline bool unlock() {
    __disable_irq();
    bool result(flag);
    flag = false;
    __enable_irq();
    return result;
  }
  
private:
  bool flag;
};
