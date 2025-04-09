#include <cassert>
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>
#include <list>
#include <iterator>

template<typename T>
class VectorList {
private:
    using VectT  = std::vector<T>;
    using ListT = std::list<VectT>;

public:
    using value_type = T;

    VectorList() = default;

    VectorList(VectorList &&)      = default;
    VectorList(VectorList const &) = default;

    VectorList & operator=(VectorList &&)      = default;
    VectorList & operator=(VectorList const &) = default;

    // метод, который будет использоваться для заполнения VectorList
    // гарантирует, что в списке не будет пустых массивов
    template<typename It>
    void append(It p, It q) {
        if (p != q)
            this->data_.push_back(VectT(p,q));
    }

    bool empty() const { return size() == 0; }

    // определите метод size
    size_t size() const {
        size_t size = 0;
        for (auto it: this->data_) {
            size += it.size();
        }
        return size;
    }

    // определите const_iterator
    struct const_iterator
        //: std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t,
        //const T&, const T*>
    {
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer   = const T*;
            using reference = const T&;

            const_iterator() = default;

            const_iterator(const ListT *data,
                    const typename ListT::const_iterator &it,
                    const typename VectT::const_iterator &itv)
                : it(it), itv(itv), data_(data) {}

            const_iterator(const const_iterator &other) = default;
            const_iterator &operator=(const const_iterator &other)
                = default;

            const_iterator &operator++() {
                if (++this->itv == this->it->end()) {
                    if (this->it != --this->data_->end()) {
                        this->itv = (++this->it)->begin();
                    }
                }

                return *this;
            }

            const_iterator operator++(int) {
                const_iterator ret(*this);
                ++(*this);
                return ret;
            }

            const_iterator &operator--() {
                if (this->itv == this->it->begin()) {
                    // Move to the previous vector if not at the first valid vector
                    if (this->it != ++this->data_->begin()) {
                        --this->it;
                        this->itv = this->it->end();
                    }
                }
                --this->itv; // Decrement the vector iterator
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator ret(*this);
                --(*this);
                return ret;
            }

            bool operator!=(const const_iterator &other) const {
                return this->itv != other.itv;
            }

            bool operator==(const const_iterator &other) const {
                return this->itv == other.itv;
            }

            reference operator*() const {
                return *this->itv;
            }

            pointer operator->() const {
                return std::addressof(*this->itv);
            }

    private:
            typename ListT::const_iterator it;
            typename VectT::const_iterator itv;
            const ListT *data_;
    };

    // определите методы begin / end
    const_iterator begin() const {
        return const_iterator(&this->data_, ++this->data_.begin(),
                (this->data_.size() == 1 ? this->data_.begin() : ++this->data_.begin())->begin());
    }

    const_iterator end() const {
        return const_iterator(&this->data_, --this->data_.end(),
                (--this->data_.end())->end());
    }

    // определите const_reverse_iterator
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // определите методы rbegin / rend
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(this->end());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(this->begin());
    }

private:
    ListT data_ = { VectT{} };
};


#if 0
int main(int /*argc*/, char * /*argv*/ []) {
    VectorList<int> vl;

    std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7};

    vl.append(v.begin(), v.begin() + 4);
    vl.append(v.begin() + 4, v.end());

    std::cout << v.size() << std::endl;
    std::cout << vl.size() << std::endl;

#if 1
    for (auto it = vl.begin(); it != vl.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

# if 0
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
# endif

    for (auto it = vl.rbegin(); it != vl.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
#endif

    VectorList<int> vl2;

    auto b = vl2.begin();
    auto e = vl2.end();

    std::cout << (b == e) << std::endl;

    return 0;
}
#endif

#if 1
int main()
{
    VectorList<int> vlist;

    std::vector<int> expectedItems = { 1,2,3,4,5,6,7 };

    std::vector<int> v6;
    v6.push_back(1);
    v6.push_back(2);
    v6.push_back(3);

    std::vector<int> v2;
    v2.push_back(4);
    v2.push_back(5);
    v2.push_back(6);
    v2.push_back(7);
    vlist.append(v6.begin(), v6.end());
    vlist.append(v2.begin(), v2.end());

    //Initial test group
    assert( vlist.size() == 7 );
    assert( *vlist.begin() == 1 );
    auto dist = std::distance(vlist.begin(), vlist.end());
    assert( std::distance( vlist.begin(), vlist.end() ) == 7 );

    assert( std::equal(vlist.begin() , vlist.end(), expectedItems.begin() ));

    VectorList<int> vlistEmpty;
    assert(std::distance(vlistEmpty.begin(), vlistEmpty.end()) == 0);

    std::cout << "Test i++" << std::endl;
    for ( auto i = vlist.begin(); i != vlist.end(); i++)
        std::cout << *i << " ";

    std::cout << "Test ++i" << std::endl;
    for (auto i = vlist.begin(); i != vlist.end(); ++i)
        std::cout << *i << " ";

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Test --i" << std::endl;
    for ( auto i = vlist.end(); i != vlist.begin();)
        std::cout << *--i << " ";
    std::cout << std::endl;

    std::cout << "Test i--" << std::endl;
    for (auto i = vlist.end(); i != vlist.begin();) {
        i--;
        std::cout << *i << " ";
    }
    std::cout << std::endl;


    std::cout << std::endl;
    auto j = vlist.rbegin();
    std::cout << "rbegin is " << *j << std::endl;
    j = --vlist.rend();
    std::cout << "--rend is " << *j << std::endl;

    std::cout << "Test reverse_const_iterator ++" << std::endl;
    for (j = vlist.rbegin(); j != vlist.rend(); ++j)
        std::cout << *j << " ";
    std::cout << std::endl;

    auto it1 = vlist.begin();
    for ( ; it1 != vlist.end(); ++it1 )
        std::cout << *it1 << ' ';

    std::cout << std::endl;
    std::cout << *--it1 << ' ';
    std::cout << *--it1 << ' ';
    std::cout << *--it1 << ' ';
    std::cout << *--it1 << ' ';

    std::cout << "One element test" << std::endl;
#if 0
    VectorList<int> vlistOneElement;
    std::vector<int> vOne;
    vOne.push_back( 1 );
    vlistOneElement.append(vOne.begin(), vOne.end() );

    auto it3 = vlistOneElement.begin();
    for (; it3 != vlistOneElement.end(); ++it3)
        std::cout << *it3 << ' ';

    VectorList<int> vListEmpty;
    auto it4 = vListEmpty.begin();
    for (; it4 != vListEmpty.end(); ++it4)
        std::cout << *it4 << ' ';
#endif

# if 1
    {
        VectorList<int> vListSingleItem;
        std::vector<int> vOneelemtn;
        vOneelemtn.push_back(1);
        vOneelemtn.push_back(2);
        vListSingleItem.append(vOneelemtn.begin(), vOneelemtn.end());
        vListSingleItem.append(vOneelemtn.begin(), vOneelemtn.end());

        auto itcontaint = vListSingleItem.begin();
        for (; itcontaint != vListSingleItem.end(); ++itcontaint)
            std::cout << *itcontaint << ' ';

        --itcontaint;
        std::cout << *itcontaint << ' ';
        --itcontaint;
        std::cout << *itcontaint << ' ';
        --itcontaint;
        std::cout << *itcontaint << ' ';
        --itcontaint;
        std::cout << *itcontaint << ' ';
        --itcontaint;
        ++itcontaint;
        std::flush(std::cout);
        std::cout << *itcontaint << ' ';

    }
# endif

    {


        std::vector<std::string> v1 = { "one", "two", "three" };
        std::vector<std::string> v2 = { "four", "five", "six", "seven", "eight" };
        std::vector<std::string> v3 = { "nine", "ten", "eleven", "twelve" };
        std::vector<std::string> v4 = {};

        for (int k = 13; k <= 30; ++k) {
            v4.push_back(std::to_string(k) + "-th");
        }

        VectorList<std::string> vl;

        std::cout << "empty distance = " << std::distance(vl.rbegin(), vl.rend()) << std::endl;


        vl.append(v1.begin(), v1.end());

        vl.append(v2.begin(), v2.end());

        vl.append(v3.begin(), v3.end());

        vl.append(v4.begin(), v4.end());
        VectorList<std::string>::const_iterator it = vl.begin();
        for (; it != vl.end(); ++it) { std::string a = *it;   std::cout << a << " "; }std::cout << std::endl; std::cout << "distance = " << std::distance(vl.begin(), vl.end()) << std::endl;
        VectorList<std::string>::const_iterator eit = vl.end();
        for (; eit != vl.begin();) {
            std::string a = *(--eit);
            std::cout << a << " ";
        }std::cout << std::endl;
        VectorList<std::string>::const_reverse_iterator rit = vl.rend();
        for (; rit != vl.rend(); ++rit) {
            std::string a = *rit;   std::cout << a << " ";
        }std::cout << std::endl; std::cout << "reverse distance = " << std::distance(vl.rbegin(), vl.rend()) << std::endl;
        VectorList<std::string>::const_reverse_iterator erit = vl.rend();
        for (; erit != vl.rbegin();) {
            std::string a = *(--erit);   std::cout << a << " ";
        }std::cout << std::endl;
        VectorList<std::string>::const_iterator i = vl.rend().base();
        for (; i != vl.end(); ++i) {
            std::string a = *i;
            std::cout << a << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
#endif
