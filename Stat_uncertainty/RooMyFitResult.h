/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooMyFitResult.h,v 1.28 2007/05/11 09:11:30 verkerke Exp $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/
#ifndef ROO_MYFIT_RESULT
#define ROO_MYFIT_RESULT

#include "Riosfwd.h"
#include "RooAbsArg.h"
#include "RooPrintable.h"
#include "RooDirItem.h"
#include "RooArgList.h"

#include "RVersion.h"
#if ROOT_VERSION_CODE >= 327680
#include "TMatrixFfwd.h"
#include "TRootIOCtor.h"
#else
class TMatrixF;
#endif

class RooArgSet ;
class RooPlot;
class TObject ;
typedef RooArgSet* pRooArgSet ;

class RooMyFitResult : public TNamed, public RooPrintable, public RooDirItem {
public:
 
  // Constructors, assignment etc.
  RooMyFitResult(const char* name=0, const char* title=0) ;
  RooMyFitResult(const RooMyFitResult& other) ;   				// added, FMV 08/13/03
  virtual TObject* clone() const { return new RooMyFitResult(*this); }   	// added, FMV 08/13/03
  virtual ~RooMyFitResult() ;

  static RooMyFitResult* lastMinuitFit(const RooArgList& varList=RooArgList()) ;

  // Printing interface (human readable)
  virtual void printValue(ostream& os) const ;
  virtual void printValue() const ;
  virtual void printName(ostream& os) const ;
  virtual void printTitle(ostream& os) const ;
  virtual void printClassName(ostream& os) const ;
  virtual void printArgs(ostream& os) const ;
  void printMultiline(ostream& os, Int_t contents, Bool_t verbose=kFALSE, TString indent="") const ;

  inline virtual void Print(Option_t *options= 0) const {
    // Printing interface
    printStream(defaultPrintStream(),defaultPrintContents(options),defaultPrintStyle(options));
  }

  virtual Int_t defaultPrintContents(Option_t* opt) const ;
  virtual StyleOption defaultPrintStyle(Option_t* opt) const ;



  // Accessors
  inline Int_t status() const {
    // Return MINUIT status code
    return _status ; 
  }
  inline Int_t covQual() const { 
    // Return MINUIT quality code of covariance matrix
    return _covQual ; 
  }
  inline Int_t numInvalidNLL() const { 
    // Return number of NLL evaluations with problems
    return _numBadNLL ; 
  }
  inline Double_t edm() const { 
    // Return estimated distance to minimum
    return _edm ; 
  }
  inline Double_t minNll() const { 
    // Return minimized -log(L) value
    return _minNLL ; 
  }
  inline const RooArgList& constPars() const { 
    // Return list of constant parameters
    return *_constPars ; 
  }
  inline const RooArgList& floatParsInit() const { 
    // Return list of floating parameters before fit
    return *_initPars ; 
  } 
  inline const RooArgList& floatParsFinal() const { 
    // Return list of floarting parameters after fit
    return *_finalPars ; 
  } 

  Double_t correlation(const RooAbsArg& par1, const RooAbsArg& par2) const {
    // Return correlation between par1 and par2
    return correlation(par1.GetName(),par2.GetName()) ;
  }
  const RooArgList* correlation(const RooAbsArg& par) const {
    // Return pointer to list of correlations of all parameters with par
    return correlation(par.GetName()) ;
  }

  Double_t correlation(const char* parname1, const char* parname2) const ;
  const RooArgList* correlation(const char* parname) const ;

  // Global correlation accessors
  Double_t globalCorr(const RooAbsArg& par) { return globalCorr(par.GetName()) ; }
  Double_t globalCorr(const char* parname) ;
  const RooArgList* globalCorr() ;


  // Add objects to a 2D plot
  inline RooPlot *plotOn(RooPlot *frame, const RooAbsArg &par1, const RooAbsArg &par2,
			 const char *options= "ME") const {
    // Plot error ellipse in par1 and par2 on frame
    return plotOn(frame,par1.GetName(),par2.GetName(),options);
  }
  RooPlot *plotOn(RooPlot *plot, const char *parName1, const char *parName2,
		  const char *options= "ME") const;

  // Generate random perturbations of the final parameters using the covariance matrix
  const RooArgList& randomizePars() const;

  Bool_t isIdentical(const RooMyFitResult& other, Double_t tol=5e-5, Double_t tolCorr=1e-4, Bool_t verbose=kTRUE) const ;

  void SetName(const char *name) ;
  void SetNameTitle(const char *name, const char* title) ;

  void setConstParList(const RooArgList& list) ;
  void setInitParList(const RooArgList& list) ;
  void setFinalParList(const RooArgList& list) ;
  inline void setMinNLL(Double_t val) { _minNLL = val ; }
  inline void setEDM(Double_t val) { _edm = val ; }
  inline void setStatus(Int_t val) { _status = val ; }
  inline void setCovQual(Int_t val) { _covQual = val ; }
  inline void setNumInvalidNLL(Int_t val) { _numBadNLL=val ; }
  void fillCorrMatrix() ;
  void MyfillCorrMatrix(Int_t Npar,vector<double> GlobalCorre,vector<double> ErrorMatrix );
  void setGlobalCorr(const RooArgList & list);
  void setCorrMatrix(const TList & list);
  Double_t correlation(Int_t row, Int_t col) const;
  Double_t covariance(Int_t row, Int_t col) const;

protected:
  
  friend class RooMinuit ;
  friend class RooNag ;
//  void setConstParList(const RooArgList& list) ;
//  void setInitParList(const RooArgList& list) ;
//  void setFinalParList(const RooArgList& list) ;
//  inline void setMinNLL(Double_t val) { _minNLL = val ; }
//  inline void setEDM(Double_t val) { _edm = val ; }
//  inline void setStatus(Int_t val) { _status = val ; }
//  inline void setCovQual(Int_t val) { _covQual = val ; }
//  inline void setNumInvalidNLL(Int_t val) { _numBadNLL=val ; }
//  void fillCorrMatrix() ;

  //Double_t correlation(Int_t row, Int_t col) const;
  //Double_t covariance(Int_t row, Int_t col) const;

  Int_t    _status ;          // MINUIT status code
  Int_t    _covQual ;         // MINUIT quality code of covariance matrix
  Int_t    _numBadNLL ;       // Number calls with bad (zero,negative) likelihood 
  Double_t _minNLL ;          // NLL at minimum
  Double_t _edm ;             // Estimated distance to minimum
  RooArgList* _constPars ;    // List of constant parameters
  RooArgList* _initPars ;     // List of floating parameters with initial values
  RooArgList* _finalPars ;    // List of floating parameters with final values
  RooArgList* _globalCorr ;   // List of global correlation coefficients
  TList       _corrMatrix ;   // Correlation matrix (list of RooArgLists)

  mutable RooArgList *_randomPars; //! List of floating parameters with most recent random perturbation applied
  mutable TMatrixF* _Lt;            //! triangular matrix used for generate random perturbations

  ClassDef(RooMyFitResult,1) // Container class for fit result
};

#endif