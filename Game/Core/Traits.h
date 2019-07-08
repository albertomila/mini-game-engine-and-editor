#pragma once

///////////////////////////////////////STATIC IF ELSE

template <bool Condition,  class CaseTrue, class CaseFalse>
struct static_if_else;

template <class CaseTrue, class CaseFalse>
struct static_if_else <true, CaseTrue, CaseFalse>
{
    typedef CaseTrue type;
};
template <class CaseTrue, class CaseFalse>
struct static_if_else <false, CaseTrue, CaseFalse>
{
    typedef CaseFalse type;
};
struct SEmpty {};

///////////////////////////////////////STypeList
template <class T, class U>
struct STypeList
{
    typedef T TNode;
    typedef U TNext;
};

//////////////////////////////////////////SIZE DE LE LISTE DE TYPES
template <class>
struct static_length
{
    enum { VAL = 0 };
};

template <>
struct static_length<SEmpty>
{
    enum { VAL = 0 };
};

template <class T, class Q>
struct static_length< STypeList<T, Q> >
{
    enum { VAL = 1 + static_length<Q>::VAL };
};

///////////////////////////////////////////GET TYPE BY INDEX

template <class TList, unsigned int i>
struct type_par_indice
{
    typedef TList type;
};
template <class T, class Q>
struct type_par_indice<STypeList<T,Q>,0>
{
    typedef T type;
};

template <class T, class Q, unsigned int i>
struct type_par_indice<STypeList<T,Q>,i>
{
    typedef typename
        type_par_indice<Q,i-1>::type type;
};

///////////////////////////////////////////GET INDEX BY TYPE
template <class TList, class T>
struct indice_par_type
{
    enum { VAL = -1 };
};

template <class T>
struct indice_par_type<SEmpty, T>
{
    enum { VAL = -1 };
};

template <class T, class Q>
struct indice_par_type<STypeList<T, Q>, T>
{
    enum { VAL = 0 };
};

template <class T, class Q, class U>
struct indice_par_type <STypeList<T, Q>, U>
{
private: // constante privée, une sorte de temporaire
    enum
    {
        pos_dans_queue = indice_par_type<Q, U>::VAL
    };
public: // l'indice en tant que tel
    enum
    {
        VAL = pos_dans_queue == -1 ? -1 : 1 + pos_dans_queue
    };
};