class DebugStream : public Stream
{
  public:

    size_t write(uint8_t)
    {
      return 0;
    };
    int available()
    {
      return bufctr - rdctr;
    };
    int read()
    {
      int x;
      x = buf[rdctr];
      rdctr++;
      return x;
    };
    int peek()
    {
      int x;
      x = buf[rdctr];
      return x;
    };

    void addByte(byte b)
    {
      buf[bufctr] = b;
      bufctr++;
    }
    void addBytes(int _size,byte b[])
    {
      memset(buf,0,64);
      bufctr = 0;
      rdctr = 0;
      for (int i = 0; i < 64; i++)
      {
        if (i >= _size) return;
        addByte(b[i]);
      };
      
    }
    
    
  private:
    char  buf[64];
    int8_t bufctr = 0;
    int8_t rdctr = 0;


};
