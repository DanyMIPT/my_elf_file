
#include <initializer_list>
#include <vector>



template<typename TypeValue>
class Base
{
protected:
    TypeValue* start;
    TypeValue* finish;
    TypeValue* storage;

    static constexpr size_t base_size = 10;

    Base  ();
    ~Base ();
};


template<typename TypeValue>
class my_vector: protected Base<TypeValue>
{
private:

    size_t delta = 2;


    class my_vec_iterator: public std::iterator<std::input_iterator_tag, TypeValue>
    {
        friend class my_vector<TypeValue>;
    private:
        explicit my_vec_iterator (TypeValue* pointer);

    public:
        my_vec_iterator (const my_vec_iterator &it);

        bool operator!= (my_vec_iterator const& it) const;
        TypeValue& operator*() const;
        my_vec_iterator& operator++();

    private:
        TypeValue* p;
    };


public:

    typedef my_vec_iterator iterator;

    iterator begin();
    iterator end();


    my_vector (const std::initializer_list<TypeValue> &list);

    void realloc_my_vector ();

    TypeValue& operator [] (const size_t i) { return *(this->begin + i); }
};

template <typename TypeValue>
my_vector <TypeValue>::my_vec_iterator::my_vec_iterator (TypeValue* p):
    p (p)
    { }

template <typename TypeValue>
my_vector <TypeValue>::my_vec_iterator::my_vec_iterator (const my_vector::my_vec_iterator &it):
    p (it.p)
    { }

template <typename TypeValue>
bool my_vector <TypeValue>::my_vec_iterator::operator!= (const my_vector::my_vec_iterator &it) const
{
    return p != it.p;
}

template <typename TypeValue>
TypeValue& my_vector <TypeValue>::my_vec_iterator::operator* () const
{
    return *p;
}

template <typename TypeValue>
typename my_vector<TypeValue>::my_vec_iterator &my_vector <TypeValue>::my_vec_iterator::operator++ ()
{
    ++p;
    return *this;
}


template<typename TypeValue>
Base<TypeValue>::Base()
{
    start   = new TypeValue[base_size];
    storage = start;
    finish  = start + base_size;
}

template<typename TypeValue>
void my_vector<TypeValue>::realloc_my_vector ()
{
    size_t old_size = this->finish - this->start;
    TypeValue* old_pointer = this->start;

    this->start = new TypeValue[old_size*delta];

    for (size_t i = 0; i < old_size; i++)
        this->start[i] = old_pointer[i];

    this->finish = this->start + old_size*delta;
    this->storage = this->start + old_size;

    delete [] old_pointer;
}

template<typename TypeValue>
my_vector<TypeValue>::my_vector (const std::initializer_list<TypeValue> &list)
{
    for (auto& element : list)
    {

        if (this->storage == this->finish)
            realloc_my_vector ();

        *this->storage = element;
        this->storage++;

    }
}

template <typename TypeValue>
typename my_vector<TypeValue>::iterator my_vector <TypeValue>::begin ()
{
    return my_vector::iterator (this->start);
}

template <typename TypeValue>
typename my_vector<TypeValue>::iterator my_vector <TypeValue>::end ()
{
    return my_vector::iterator (this->storage);
}

template <typename TypeValue>
Base <TypeValue>::~Base ()
{
    delete [] start;
}
