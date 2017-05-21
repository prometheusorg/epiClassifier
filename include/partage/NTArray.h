/*
** NAUTILUS
** class NTArray
** Tabun
*/

#ifndef _NT_ARRAY_
# define _NT_ARRAY_

#include <vector>

/*
** Represent an array in Nautilus Spirit
** TYPE represent type to create
*/
template <class TYPE>
class NTArray : public std::vector<TYPE*>
{
  public:

  /*
  ** Default constructeur : do nothing
  */
  NTArray()
  {
  }

  /*
  ** Copy contructor
  **  ! Need a copy constructor in the contained's class
  */
  NTArray(NTArray& temp)
  {
    if (temp.empty())
      return ;

    typename std::vector<TYPE*>::iterator iter ;

    for (iter = temp.begin() ; temp.end() != iter ; iter++)
      this->push_back(new TYPE(**iter)) ;
  }

  /*
  ** Delete all elements of the vector
  */
  virtual void vider()
  {
    if (this->empty())
      return ;

    typename std::vector<TYPE*>::iterator iter ;

    for (iter = this->begin() ; this->end() != iter ; )
    {
      if (*iter)
        delete(*iter) ;
      this->erase(iter) ;
    }
  }

  /*
  ** operator ==
  ** call the operator == of the conatin's class
  */
  int operator == (NTArray& temp)
  {
    if (temp.size() != this->size())
      return 0 ;

    for (register size_t i =0; i < temp.size(); i++)
      if ((*temp[i] == *(*this)[i]) == false)
        return 0 ;

    return 1 ;
  }

  /*
  ** affection operator
  */
  NTArray& operator=(const NTArray& temp)
  {
  	if (this == &temp)
    	return *this ;

    vider() ;

    if (false == temp.empty())
    {
      typename std::vector<TYPE*>::const_iterator iter ;

      for (iter = temp.begin() ; temp.end() != iter ; iter++)
      	this->push_back(new TYPE(**iter)) ;
    }

    return *this ;
  }

  /*
  ** Delete all the element of the class
  */
  virtual ~NTArray()
  {
    vider() ;
  }
};

#endif

