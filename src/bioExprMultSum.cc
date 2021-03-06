//-*-c++-*------------------------------------------------------------
//
// File name : bioExprMultSum.cc
// @date   Wed Apr 18 11:05:06 2018
// @author Michel Bierlaire
// @version Revision 1.0
//
//--------------------------------------------------------------------

#include <sstream>
#include "bioSmartPointer.h"
#include "bioDebug.h"
#include "bioExprMultSum.h"
#include "bioExceptions.h"

bioExprMultSum::bioExprMultSum(std::vector<bioSmartPointer<bioExpression> > e) :
  expressions(e) {
  listOfChildren = e ;
}

bioExprMultSum::~bioExprMultSum() {
}

bioSmartPointer<bioDerivatives>
bioExprMultSum::getValueAndDerivatives(std::vector<bioUInt> literalIds,
				       bioBoolean gradient,
				       bioBoolean hessian) {

  if (!gradient && hessian) {
    throw bioExceptions(__FILE__,__LINE__,"If the hessian is needed, the gradient must be computed") ;
  }

  theDerivatives = bioSmartPointer<bioDerivatives>(new bioDerivatives(literalIds.size())) ;

  theDerivatives->f = 0.0 ;
  if (gradient) {
    if (hessian) {
      theDerivatives->setDerivativesToZero() ;
    }
    else {
      theDerivatives->setGradientToZero() ;
    }
  }
  for (std::vector<bioSmartPointer<bioExpression> >::iterator i = expressions.begin();
       i != expressions.end() ;
       ++i) {
    bioSmartPointer<bioDerivatives> fgh = (*i)->getValueAndDerivatives(literalIds,gradient,hessian) ;
    theDerivatives->f += fgh->f ;
    if (gradient) {
      for (std::size_t k = 0 ; k < literalIds.size() ; ++k) {
	theDerivatives->g[k] += fgh->g[k] ;
	if (hessian) {
	  for (std::size_t l = k ; l < literalIds.size() ; ++l) {
	    theDerivatives->h[k][l] += fgh->h[k][l] ;
	  }
	}
      }
    }
  }
  if (hessian) {
    // Fill the symmetric part of the matrix
    for (std::size_t k = 0 ; k < literalIds.size() ; ++k) {
      for (std::size_t l = k+1 ; l < literalIds.size() ; ++l) {
	theDerivatives->h[l][k]  = theDerivatives->h[k][l] ;
      }
    }
  }
  return theDerivatives ;
}

bioString bioExprMultSum::print(bioBoolean hp) const {
  std::stringstream str ;
  if (hp) {
    str << "MultiSum(" ;
    for (std::vector<bioSmartPointer<bioExpression> >::const_iterator i = expressions.begin() ;
	 i != expressions.end() ;
	 ++i) {
      if (i != expressions.begin()) {
	str << " , " ;
      }
      str << (*i)->print(hp) ;
    }
    str << ")" ;
  }
  else {
    str << "(" ;
    for (std::vector<bioSmartPointer<bioExpression> >::const_iterator i = expressions.begin() ;
	 i != expressions.end() ;
	 ++i) {
      if (i != expressions.begin()) {
	str << " + " ;
      }
      str << (*i)->print(hp) ;
    }
    str << ")" ;
  }
  return str.str() ;
}
