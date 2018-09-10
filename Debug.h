#ifndef Debug_h
#define Debug_h

// From https://playground.arduino.cc/Main/StreamingOutput
template<class T> inline Print& operator<<(Print &obj, T arg)
{
  obj.print(arg);
  return obj;
}

#ifdef DEBUG_ON
# define Debug Serial
#else
class DummyStream {
  public:
    template<class T> DummyStream& operator<<(T arg)
    {
      (void) arg;
      return *this;
    }
} Debug;
#endif

#endif

