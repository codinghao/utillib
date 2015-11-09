#define TEMPLETES_PARAM MAKE_PARAM1_N(DELEGATE_PARAM_NUM, class T)
#define TEMPLETES_ARG   MAKE_PARAM1_N(DELEGATE_PARAM_NUM, T)
#define FUNCTIONS_PARAM MAKE_PARAM2_N(DELEGATE_PARAM_NUM, T, a)
#define FUNCTIONS_ARG   MAKE_PARAM1_N(DELEGATE_PARAM_NUM, a)

template<class R, TEMPLETES_PARAM>
class Delegate<R (TEMPLETES_ARG)>
{
private:
    class DelegateImplBase
    {
    public:
        virtual DelegateImplBase* Clone() = 0;
        virtual R Invoke(FUNCTIONS_PARAM) = 0;
    };

    template<class TFunctor>
    class DelegateImpl : public DelegateImplBase
    {
    public:
        DelegateImpl(const TFunctor& _functor)
            : m_Functor(_functor)
        {}

        DelegateImplBase* Clone()
        {
            return new DelegateImpl<TFunctor>(m_Functor);
        }

        R Invoke(FUNCTIONS_PARAM)
        {
            return (*m_Functor)(FUNCTIONS_ARG);
        }

    private:
        TFunctor m_Functor;
    };

    template<class T, class TFunctor>
    class DelegateImplX: public DelegateImplBase
    {
    public:
        DelegateImplX(std::pair<T, TFunctor> _functor)
            : m_Functor(_functor)
        {}
        
        DelegateImplBase* Clone()
        {
            return new DelegateImplX<T, TFunctor>(m_Functor);
        }

        R Invoke(FUNCTIONS_PARAM)
        {
            return ((*m_Functor.first).*m_Functor.second)(FUNCTIONS_ARG);
        }

    private:
        std::pair<T, TFunctor> m_Functor;
    };

    void FreeDelegate()
    {
        if (m_pImpl != NULL)
        {
            delete m_pImpl;
            m_pImpl = NULL;
        }
    }

public:
    Delegate()
        : m_pImpl(NULL)
    {

    }

    template<class T, class TFunctor>
    Delegate(const T& _obj, const TFunctor _functor)
        : m_pImpl(new DelegateImplX<T, TFunctor>(std::make_pair(_obj, _functor)))
    {}

    template<class TFunctor>
    Delegate(const TFunctor& _functor)
        : m_pImpl(new DelegateImpl<TFunctor>(_functor))
    {

    }

    Delegate(const Delegate& _delegate)
        : m_pImpl(NULL)
    {
        *this = _delegate;
    }

    ~Delegate()
    {
        FreeDelegate();
    }

    R Invoke(FUNCTIONS_PARAM)
    {
        return m_pImpl->Invoke(FUNCTIONS_ARG);
    }

    R operator () (FUNCTIONS_PARAM)
    {
        return m_pImpl->Invoke(FUNCTIONS_ARG);
    }

    Delegate& operator = (const Delegate& _delegate)
    {
        if (this == &_delegate)
            return *this;

        FreeDelegate();
        m_pImpl = _delegate.m_pImpl->Clone();

        return *this;
    }

    template<class TFunctor>
    Delegate& operator = (const TFunctor& _functor)
    {
        FreeDelegate();
        m_pImpl = new DelegateImpl<TFunctor>( _functor);

        return *this;
    }

    template<class T, class TFunctor>
    Delegate& operator = (const std::pair<T, TFunctor>& _pair)
    {
        FreeDelegate();
        m_pImpl = new DelegateImpl<std::pair<T, TFunctor> >(_pair);

        return *this;
    }

private:
    DelegateImplBase* m_pImpl;
};

#undef TEMPLETES_PARAM
#undef TEMPLETES_ARG
#undef FUNCTIONS_PARAM
#undef FUNCTIONS_ARG
