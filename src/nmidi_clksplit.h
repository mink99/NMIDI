#ifndef NMIFI_CLKSPLIT_H
#define NMIFI_CLKSPLIT_H

class ClockFilter
{
  public:
    ClockFilter( bool use44 = true)
    {
      if (!use44) fullBarTicks = 72;  // 3-4 beats;
    }
    void reset()
    {
      beatCounter = 0;
      elapsedTicks = 0;
    }
    long bars()
    {
      return elapsedTicks%fullBarTicks;
    }
    long ticks()
    {
      return elapsedTicks;
    }
    void clockEvent()
    {
      if (fullBarTicks == 96)
      {
        if (beatCounter % 3 == 0)   on32();
        if (beatCounter % 6 == 0)   on16();
        if (beatCounter % 8 == 0)   on8T();
        if (beatCounter % 12 == 0)  on8();
        if (beatCounter % 24 == 0)  on4();
        if (beatCounter % 48 == 0)  on2();        
        if (beatCounter % 96 == 0)  on1();       
      }
      else // (fullBarTicks == 72)
      {
        if (beatCounter % 3 == 0)   on32();
        if (beatCounter % 6 == 0)   on16();
        if (beatCounter % 8 == 0)   on8T();
        if (beatCounter % 12 == 0) on8();
        if (beatCounter % 24 == 0) on4();
        if (beatCounter % 36 == 0) on2();        
        if (beatCounter % 72 == 0) on1();       
      }
      if (beatCounter > fullBarTicks) beatCounter = 0;
      beatCounter++;
      elapsedTicks++;
    }
    virtual void on1() { }
    virtual void on2() { }
    virtual void on4() { }
    virtual void on8() { }
    virtual void on8T() { }
    virtual void on16() { }
    virtual void on32() { }


  protected:
    int beatCounter = 0;
    long elapsedTicks = 0;
    int fullBarTicks = 96;
  private:

};



#endif
