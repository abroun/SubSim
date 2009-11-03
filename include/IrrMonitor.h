//------------------------------------------------------------------------------
// File: IrrMonitor.h
// Desc: Irrlicht monitor class to perform RAII on Irrlicht objects
//
// Usage: Once an Irrlicht object has been created or 'grabbed' a monitor
//        object should be created to hold the pointer. This means that
//        the object will be automatically released when the monitor goes
//        out of scope. It also means that the scope of the monitor should
//        be the scope of the object.
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<typename T>
class IrrMonitor
{
    public: IrrMonitor( T* pObj ) { mpObj = pObj; }
    public: ~IrrMonitor() 
    { 
        if ( NULL != mpObj )
        {
            mpObj->drop(); 
            mpObj = NULL; 
        }
    }

    public: T* GetObject() const { return mpObj; }

    private: T* mpObj;
};

//------------------------------------------------------------------------------
