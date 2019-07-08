#pragma once

template< typename TARG1 >
class ICallbackMember1
{
public:
    virtual void Call( TARG1& arg1 ) = 0;
    virtual bool operator == ( const ICallbackMember1& other ) const{ return false; }
};

template< typename TOBJ, typename TFCT, typename TARG1 >
class CCallbackMember1 : public ICallbackMember1<TARG1>
{
    typedef CCallbackMember1<TOBJ, TFCT, TARG1 > self;
public:
    CCallbackMember1( TOBJ* object, TFCT method )
        : m_method( method )
        , m_object( object )
    {
    }

    virtual void Call( TARG1& arg1 ) override
    {
        (m_object->*m_method)( arg1 );
    }
    
    virtual void operator = ( const CCallbackMember1<TOBJ, TFCT, TARG1 >& callbackMember )
    {
        m_object = callbackMember.m_object;
        m_method = callbackMember.m_method;
    }

    virtual bool operator == ( const ICallbackMember1& other ) const
    {
        const self* otherPtr = dynamic_cast< const self*>( &other );
        if( otherPtr )
        {
            return m_object == otherPtr->m_object;
        }

        return false;
    }
    
private:
    TOBJ* m_object;
    TFCT m_method;
};

template< typename TARG1 >
class CCallback
{
public:
    CCallback()
        : m_callbackMember( NULL )
    {
    }
    
    CCallback( const CCallback& callback )
    {
        m_callbackMember = callback.m_callbackMember;
    }

    virtual void operator = ( const CCallback& callback )
    {
        m_callbackMember = callback.m_callbackMember;
    }
    
    template< typename TOBJ, typename TFCT>
    CCallback( TOBJ* object, TFCT method )
    {
        m_callbackMember = new CCallbackMember1<TOBJ, TFCT, TARG1>( object, method );
    }

    ~CCallback()
    {
        //delete m_callbackMember;
    }

    void Call( TARG1& arg1 )
    {
        m_callbackMember->Call( arg1 );
    }

    bool operator == ( const CCallback& other ) const
    {
        return *m_callbackMember == *(other.m_callbackMember);
    }
private:
    ICallbackMember1<TARG1>* m_callbackMember;
};


template< typename TARG1, typename TARG2 >
class ICallbackMember2
{
public:
    virtual void Call( TARG1& arg1, TARG2& arg2 ) = 0;
};

template< typename TOBJ, typename TFCT, typename TARG1, typename TARG2 >
class CCallbackMember2 : public ICallbackMember2<TARG1, TARG2>
{
public:
    CCallbackMember2( TOBJ* object, TFCT method )
        : m_method( method )
        , m_object( object )
    {
    }

    virtual void Call( TARG1& arg1, TARG2& arg2 ) override
    {
        (m_object->*m_method)( arg1, arg2 );
    }

    virtual void operator = ( const CCallbackMember2<TOBJ, TFCT, TARG1, TARG2 >& callbackMember )
    {
        m_object = callbackMember.m_object;
        m_method = callbackMember.m_method;
    }

private:
    TOBJ* m_object;
    TFCT m_method;
};

template< typename TARG1, typename TARG2 >
class CCallback2
{
public:
    CCallback2()
        : m_callbackMember( NULL )
    {
    }

    CCallback2( const CCallback2& callback )
    {
        m_callbackMember = callback.m_callbackMember;
    }

    virtual void operator = ( const CCallback2& callback )
    {
        m_callbackMember = callback.m_callbackMember;
    }

    template< typename TOBJ, typename TFCT>
    CCallback2( TOBJ* object, TFCT method )
    {
        m_callbackMember = new CCallbackMember2<TOBJ, TFCT, TARG1, TARG2>( object, method );
    }

    ~CCallback2()
    {
        //delete m_callbackMember;
    }

    void Call( TARG1& arg1, TARG2& arg2 )
    {
        m_callbackMember->Call( arg1, arg2 );
    }

private:
	ICallbackMember2<TARG1, TARG2>* m_callbackMember;
};