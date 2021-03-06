//-*-c++-*------------------------------------------------------------
//
// File name : bioExprGreater.h
// @date   Thu Apr 19 07:27:12 2018
// @author Michel Bierlaire
// @version Revision 1.0
//
//--------------------------------------------------------------------

#ifndef bioExprGreater_h
#define bioExprGreater_h

#include "bioExpression.h"
#include "bioString.h"

class bioExprGreater: public bioExpression {
 public:
  bioExprGreater(bioSmartPointer<bioExpression>  l, bioSmartPointer<bioExpression>  r) ;
  ~bioExprGreater() ;
  virtual bioSmartPointer<bioDerivatives> getValueAndDerivatives(std::vector<bioUInt> literalIds,
								 bioBoolean gradient,
								 bioBoolean hessian) ;

  virtual bioString print(bioBoolean hp = false) const ;
 protected:
  bioSmartPointer<bioExpression>  left ;
  bioSmartPointer<bioExpression>  right ;
};


#endif
