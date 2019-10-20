#ifndef BSATOOL_SINGLETON_H
#define BSATOOL_SINGLETON_H

/**
 * @Brief Design pattern singleton
 *
 * The current design requires that the implementation declare only one
 * constructor taking only a token parameter (this ensures that no instance are
 * possible outside the singleton)
 */
template <typename T> class Singleton
{
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief copy constructor : forbidden to use
     * @param singleton singleton to copy
     */
    Singleton(const Singleton &singleton) = delete;

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * @brief operator = assignment
     * @param singleton to copy
     * @return nothing : forbidden to use
     */
    T &operator=(const T &singleton) = delete;
    /**
     * @brief Allowing to retrieve the only instance of the class
     * @return the only instance
     */
    static T& getInstance();

protected:
    //**************************************************************************
    // Constructors/Destructor
    //**************************************************************************
    /**
     * @brief Singleton constructor
     */
    Singleton() = default;

    /**
     * @brief Destructor
     */
    virtual ~Singleton() = default;

    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief Ensure that no instance are possible outside the singleton scope
     */
    struct token {};
};

//**************************************************************************
// Methods
//**************************************************************************
template<typename T>
T &Singleton<T>::getInstance()
{
    static T instance(token{});
    return instance;
}

#endif // BSATOOL_SINGLETON_H
