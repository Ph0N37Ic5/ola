/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Callback.h
 * Callback classes, these are similar to closures but can take arguments at
 * exec time.
 * Copyright (C) 2005-2010 Simon Newton
 */

#ifndef INCLUDE_OLA_CALLBACK_H_
#define INCLUDE_OLA_CALLBACK_H_

namespace ola {

// 1 argument callbacks
template <typename ReturnType, typename Arg0>
class BaseCallback1 {
  public:
    virtual ~BaseCallback1() {}
    virtual ReturnType Run(Arg0 arg0) = 0;
    virtual ReturnType DoRun(Arg0 arg0) = 0;
};


/*
 * A callback, this can be called multiple times
 */
template <typename ReturnType, typename Arg0>
class Callback1: public BaseCallback1<ReturnType, Arg0> {
  public:
    virtual ~Callback1() {}
    ReturnType Run(Arg0 arg0) { return DoRun(arg0); }
};


/*
 * A single use callback, this deletes itself after it's run.
 */
template <typename ReturnType, typename Arg0>
class SingleUseCallback1: public BaseCallback1<ReturnType, Arg0> {
  public:
    virtual ~SingleUseCallback1() {}
    ReturnType Run(Arg0 arg0) {
      ReturnType ret = DoRun(arg0);
      delete this;
      return ret;
    }
};


/*
 * A single use callback returning void.
 */
template <typename Arg0>
class SingleUseCallback1<void, Arg0>: public BaseCallback1<void, Arg0> {
  public:
    virtual ~SingleUseCallback1() {}
    void Run(Arg0 arg0) {
      DoRun(arg0);
      delete this;
    }
};


/*
 * An method callback with 0 create-time args, and 1 exec time arg
 */
template <typename Class, typename Parent, typename ReturnType, typename Arg0>
class MethodCallback0_1: public Parent {
  public:
    typedef ReturnType (Class::*Method)(Arg0);
    MethodCallback0_1(Class *object, Method callback):
      Parent(),
      m_object(object),
      m_callback(callback) {}
    ReturnType DoRun(Arg0 arg0) {
      return (m_object->*m_callback)(arg0);
    }
  private:
    Class *m_object;
    Method m_callback;
};

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename Arg0>
inline SingleUseCallback1<ReturnType, Arg0>* NewSingleCallback(
    Class* object,
    ReturnType (Class::*method)(Arg0)) {
  return new MethodCallback0_1<Class,
                               SingleUseCallback1<ReturnType, Arg0>,
                               ReturnType,
                               Arg0>(
      object,
      method);
}

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename Arg0>
inline Callback1<ReturnType, Arg0>* NewCallback(
    Class* object,
    ReturnType (Class::*method)(Arg0)) {
  return new MethodCallback0_1<Class,
                               Callback1<ReturnType, Arg0>,
                               ReturnType,
                               Arg0>(
      object,
      method);
}

/*
 * An method callback with 1 create-time args, and 1 exec time arg
 */
template <typename Class, typename Parent, typename ReturnType, typename A0, typename Arg0>
class MethodCallback1_1: public Parent {
  public:
    typedef ReturnType (Class::*Method)(A0, Arg0);
    MethodCallback1_1(Class *object, Method callback, A0 a0):
      Parent(),
      m_object(object),
      m_callback(callback),
      m_a0(a0) {}
    ReturnType DoRun(Arg0 arg0) {
      return (m_object->*m_callback)(m_a0, arg0);
    }
  private:
    Class *m_object;
    Method m_callback;
  A0 m_a0;
};

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename A0, typename Arg0>
inline SingleUseCallback1<ReturnType, Arg0>* NewSingleCallback(
    Class* object,
    ReturnType (Class::*method)(A0, Arg0),
    A0 a0) {
  return new MethodCallback1_1<Class,
                               SingleUseCallback1<ReturnType, Arg0>,
                               ReturnType,
                               A0,
                               Arg0>(
      object,
      method,
      a0);
}

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename A0, typename Arg0>
inline Callback1<ReturnType, Arg0>* NewCallback(
    Class* object,
    ReturnType (Class::*method)(A0, Arg0),
    A0 a0) {
  return new MethodCallback1_1<Class,
                               Callback1<ReturnType, Arg0>,
                               ReturnType,
                               A0,
                               Arg0>(
      object,
      method,
      a0);
}

/*
 * An method callback with 2 create-time args, and 1 exec time arg
 */
template <typename Class, typename Parent, typename ReturnType, typename A0, typename A1, typename Arg0>
class MethodCallback2_1: public Parent {
  public:
    typedef ReturnType (Class::*Method)(A0, A1, Arg0);
    MethodCallback2_1(Class *object, Method callback, A0 a0, A1 a1):
      Parent(),
      m_object(object),
      m_callback(callback),
      m_a0(a0),
      m_a1(a1) {}
    ReturnType DoRun(Arg0 arg0) {
      return (m_object->*m_callback)(m_a0, m_a1, arg0);
    }
  private:
    Class *m_object;
    Method m_callback;
  A0 m_a0;
  A1 m_a1;
};

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename A0, typename A1, typename Arg0>
inline SingleUseCallback1<ReturnType, Arg0>* NewSingleCallback(
    Class* object,
    ReturnType (Class::*method)(A0, A1, Arg0),
    A0 a0,
    A1 a1) {
  return new MethodCallback2_1<Class,
                               SingleUseCallback1<ReturnType, Arg0>,
                               ReturnType,
                               A0,
                               A1,
                               Arg0>(
      object,
      method,
      a0,
      a1);
}

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename A0, typename A1, typename Arg0>
inline Callback1<ReturnType, Arg0>* NewCallback(
    Class* object,
    ReturnType (Class::*method)(A0, A1, Arg0),
    A0 a0,
    A1 a1) {
  return new MethodCallback2_1<Class,
                               Callback1<ReturnType, Arg0>,
                               ReturnType,
                               A0,
                               A1,
                               Arg0>(
      object,
      method,
      a0,
      a1);
}

/*
 * An method callback with 3 create-time args, and 1 exec time arg
 */
template <typename Class, typename Parent, typename ReturnType, typename A0, typename A1, typename A2, typename Arg0>
class MethodCallback3_1: public Parent {
  public:
    typedef ReturnType (Class::*Method)(A0, A1, A2, Arg0);
    MethodCallback3_1(Class *object, Method callback, A0 a0, A1 a1, A2 a2):
      Parent(),
      m_object(object),
      m_callback(callback),
      m_a0(a0),
      m_a1(a1),
      m_a2(a2) {}
    ReturnType DoRun(Arg0 arg0) {
      return (m_object->*m_callback)(m_a0, m_a1, m_a2, arg0);
    }
  private:
    Class *m_object;
    Method m_callback;
  A0 m_a0;
  A1 m_a1;
  A2 m_a2;
};

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename A0, typename A1, typename A2, typename Arg0>
inline SingleUseCallback1<ReturnType, Arg0>* NewSingleCallback(
    Class* object,
    ReturnType (Class::*method)(A0, A1, A2, Arg0),
    A0 a0,
    A1 a1,
    A2 a2) {
  return new MethodCallback3_1<Class,
                               SingleUseCallback1<ReturnType, Arg0>,
                               ReturnType,
                               A0,
                               A1,
                               A2,
                               Arg0>(
      object,
      method,
      a0,
      a1,
      a2);
}

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename A0, typename A1, typename A2, typename Arg0>
inline Callback1<ReturnType, Arg0>* NewCallback(
    Class* object,
    ReturnType (Class::*method)(A0, A1, A2, Arg0),
    A0 a0,
    A1 a1,
    A2 a2) {
  return new MethodCallback3_1<Class,
                               Callback1<ReturnType, Arg0>,
                               ReturnType,
                               A0,
                               A1,
                               A2,
                               Arg0>(
      object,
      method,
      a0,
      a1,
      a2);
}

// 2 argument callbacks
template <typename ReturnType, typename Arg0, typename Arg1>
class BaseCallback2 {
  public:
    virtual ~BaseCallback2() {}
    virtual ReturnType Run(Arg0 arg0, Arg1 arg1) = 0;
    virtual ReturnType DoRun(Arg0 arg0, Arg1 arg1) = 0;
};


/*
 * A callback, this can be called multiple times
 */
template <typename ReturnType, typename Arg0, typename Arg1>
class Callback2: public BaseCallback2<ReturnType, Arg0, Arg1> {
  public:
    virtual ~Callback2() {}
    ReturnType Run(Arg0 arg0, Arg1 arg1) { return DoRun(arg0, arg1); }
};


/*
 * A single use callback, this deletes itself after it's run.
 */
template <typename ReturnType, typename Arg0, typename Arg1>
class SingleUseCallback2: public BaseCallback2<ReturnType, Arg0, Arg1> {
  public:
    virtual ~SingleUseCallback2() {}
    ReturnType Run(Arg0 arg0, Arg1 arg1) {
      ReturnType ret = DoRun(arg0, arg1);
      delete this;
      return ret;
    }
};


/*
 * A single use callback returning void.
 */
template <typename Arg0, typename Arg1>
class SingleUseCallback2<void, Arg0, Arg1>: public BaseCallback2<void, Arg0, Arg1> {
  public:
    virtual ~SingleUseCallback2() {}
    void Run(Arg0 arg0, Arg1 arg1) {
      DoRun(arg0, arg1);
      delete this;
    }
};


/*
 * An method callback with 0 create-time args, and 2 exec time arg
 */
template <typename Class, typename Parent, typename ReturnType, typename Arg0, typename Arg1>
class MethodCallback0_2: public Parent {
  public:
    typedef ReturnType (Class::*Method)(Arg0, Arg1);
    MethodCallback0_2(Class *object, Method callback):
      Parent(),
      m_object(object),
      m_callback(callback) {}
    ReturnType DoRun(Arg0 arg0, Arg1 arg1) {
      return (m_object->*m_callback)(arg0, arg1);
    }
  private:
    Class *m_object;
    Method m_callback;
};

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename Arg0, typename Arg1>
inline SingleUseCallback2<ReturnType, Arg0, Arg1>* NewSingleCallback(
    Class* object,
    ReturnType (Class::*method)(Arg0, Arg1)) {
  return new MethodCallback0_2<Class,
                               SingleUseCallback2<ReturnType, Arg0, Arg1>,
                               ReturnType,
                               Arg0,
                               Arg1>(
      object,
      method);
}

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename Arg0, typename Arg1>
inline Callback2<ReturnType, Arg0, Arg1>* NewCallback(
    Class* object,
    ReturnType (Class::*method)(Arg0, Arg1)) {
  return new MethodCallback0_2<Class,
                               Callback2<ReturnType, Arg0, Arg1>,
                               ReturnType,
                               Arg0,
                               Arg1>(
      object,
      method);
}

/*
 * An method callback with 1 create-time args, and 2 exec time arg
 */
template <typename Class, typename Parent, typename ReturnType, typename A0, typename Arg0, typename Arg1>
class MethodCallback1_2: public Parent {
  public:
    typedef ReturnType (Class::*Method)(A0, Arg0, Arg1);
    MethodCallback1_2(Class *object, Method callback, A0 a0):
      Parent(),
      m_object(object),
      m_callback(callback),
      m_a0(a0) {}
    ReturnType DoRun(Arg0 arg0, Arg1 arg1) {
      return (m_object->*m_callback)(m_a0, arg0, arg1);
    }
  private:
    Class *m_object;
    Method m_callback;
  A0 m_a0;
};

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename A0, typename Arg0, typename Arg1>
inline SingleUseCallback2<ReturnType, Arg0, Arg1>* NewSingleCallback(
    Class* object,
    ReturnType (Class::*method)(A0, Arg0, Arg1),
    A0 a0) {
  return new MethodCallback1_2<Class,
                               SingleUseCallback2<ReturnType, Arg0, Arg1>,
                               ReturnType,
                               A0,
                               Arg0,
                               Arg1>(
      object,
      method,
      a0);
}

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename A0, typename Arg0, typename Arg1>
inline Callback2<ReturnType, Arg0, Arg1>* NewCallback(
    Class* object,
    ReturnType (Class::*method)(A0, Arg0, Arg1),
    A0 a0) {
  return new MethodCallback1_2<Class,
                               Callback2<ReturnType, Arg0, Arg1>,
                               ReturnType,
                               A0,
                               Arg0,
                               Arg1>(
      object,
      method,
      a0);
}

// 4 argument callbacks
template <typename ReturnType, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
class BaseCallback4 {
  public:
    virtual ~BaseCallback4() {}
    virtual ReturnType Run(Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3) = 0;
    virtual ReturnType DoRun(Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3) = 0;
};


/*
 * A callback, this can be called multiple times
 */
template <typename ReturnType, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
class Callback4: public BaseCallback4<ReturnType, Arg0, Arg1, Arg2, Arg3> {
  public:
    virtual ~Callback4() {}
    ReturnType Run(Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3) { return DoRun(arg0, arg1, arg2, arg3); }
};


/*
 * A single use callback, this deletes itself after it's run.
 */
template <typename ReturnType, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
class SingleUseCallback4: public BaseCallback4<ReturnType, Arg0, Arg1, Arg2, Arg3> {
  public:
    virtual ~SingleUseCallback4() {}
    ReturnType Run(Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3) {
      ReturnType ret = DoRun(arg0, arg1, arg2, arg3);
      delete this;
      return ret;
    }
};


/*
 * A single use callback returning void.
 */
template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
class SingleUseCallback4<void, Arg0, Arg1, Arg2, Arg3>: public BaseCallback4<void, Arg0, Arg1, Arg2, Arg3> {
  public:
    virtual ~SingleUseCallback4() {}
    void Run(Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3) {
      DoRun(arg0, arg1, arg2, arg3);
      delete this;
    }
};


/*
 * An method callback with 0 create-time args, and 4 exec time arg
 */
template <typename Class, typename Parent, typename ReturnType, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
class MethodCallback0_4: public Parent {
  public:
    typedef ReturnType (Class::*Method)(Arg0, Arg1, Arg2, Arg3);
    MethodCallback0_4(Class *object, Method callback):
      Parent(),
      m_object(object),
      m_callback(callback) {}
    ReturnType DoRun(Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3) {
      return (m_object->*m_callback)(arg0, arg1, arg2, arg3);
    }
  private:
    Class *m_object;
    Method m_callback;
};

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
inline SingleUseCallback4<ReturnType, Arg0, Arg1, Arg2, Arg3>* NewSingleCallback(
    Class* object,
    ReturnType (Class::*method)(Arg0, Arg1, Arg2, Arg3)) {
  return new MethodCallback0_4<Class,
                               SingleUseCallback4<ReturnType, Arg0, Arg1, Arg2, Arg3>,
                               ReturnType,
                               Arg0,
                               Arg1,
                               Arg2,
                               Arg3>(
      object,
      method);
}

/*
 * Create a new single use method callback.
 */
template <typename Class, typename ReturnType, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
inline Callback4<ReturnType, Arg0, Arg1, Arg2, Arg3>* NewCallback(
    Class* object,
    ReturnType (Class::*method)(Arg0, Arg1, Arg2, Arg3)) {
  return new MethodCallback0_4<Class,
                               Callback4<ReturnType, Arg0, Arg1, Arg2, Arg3>,
                               ReturnType,
                               Arg0,
                               Arg1,
                               Arg2,
                               Arg3>(
      object,
      method);
}

}  // ola
#endif  // INCLUDE_OLA_CALLBACK_H_
