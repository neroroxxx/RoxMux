/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  This is a simple Number scroller, you can scroll the number up or down, and just
  to be clear by scrolling i mean increase/decrease a value, you can also wrap
  the number and you can do with the accurate number when you wrap or wrap back to
  the min/max

  Use at your own risk.
*/
#ifndef RoxScroller_h
#define RoxScroller_h

template <typename T>
class RoxScroller {
  private:
    T scroll(T value, T amount, bool wrap, T min, T max, bool up, bool t_accurate){
      if(up){
        if((value + amount) > max){
          if(t_accurate){
            return (wrap) ? (min + ((value + amount) - max)) : max;
          }
          return (wrap) ? min : max;
        }
        return (value + amount);
      }
      if((value - amount) < min){
        if(t_accurate){
          return (wrap) ? (max + ((value - amount) + min)) : min;
        }
        return (wrap) ? max : min;
      }
      return (value - amount);
    }
  public:
    RoxScroller(){}
    T scroll(T &value, bool t_up, T amount, bool wrap, T min, T max){
      if(t_up){
        return up(value, amount, wrap, min, max);
      }
      return down(value, amount, wrap, min, max);
    }
    T up(T &value, T amount, bool wrap, T min, T max){
      value = scroll(value, amount, wrap, min, max, true, false);
      return value;
    }
    T down(T &value, T amount, bool wrap, T min, T max){
      value = scroll(value, amount, wrap, min, max, false, false);
      return value;
    }
    T upAccurate(T &value, T amount, bool wrap, T min, T max){
      value = scroll(value, amount, wrap, min, max, true, true);
      return value;
    }
    T downAccurate(T &value, T amount, bool wrap, T min, T max){
      value = scroll(value, amount, wrap, min, max, false, true);
      return value;
    }
};
#endif
