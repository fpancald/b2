#ifndef BERTINI_NUMBER
#define BERTINI_NUMBER

#include <iostream>
#include <random>
#include <memory>
#include <sstream>
#include <cmath>
#include <boost/multiprecision/mpfr.hpp>
#include <boost/multiprecision/random.hpp>
#include <boost/math/constants/constants.hpp>
#include <random>
#include <cmath>
#include <exception>
#include <boost/random.hpp>



unsigned int DigitsToBits(unsigned int digits){
	return ceil((digits -1)/log10(2));
}

unsigned int BitsToDigits(unsigned int bits){
	return ceil((bits -1)/log2(10));
}




template <unsigned int length_in_digits>
void RandomLongNumberSpecificDigitsUniformUnitInterval(boost::multiprecision::mpfr_float & number_to_make_random)
{
	unsigned int prior_default_precision = boost::multiprecision::mpfr_float::default_precision();
	boost::multiprecision::mpfr_float::default_precision(number_to_make_random.precision());
	
	static boost::uniform_01<boost::multiprecision::mpfr_float> uf;
	static boost::random::independent_bits_engine<boost::random::mt19937, length_in_digits*1000L/301L, boost::multiprecision::mpz_int> gen;
	number_to_make_random = uf(gen);
	
	boost::multiprecision::mpfr_float::default_precision(prior_default_precision);

	
	return;
}




void RandomLongNumberUniformUnitInterval(unsigned int num_digits, boost::multiprecision::mpfr_float & number_to_make_random)
{
	if (num_digits<=50)
		return RandomLongNumberSpecificDigitsUniformUnitInterval<50>(number_to_make_random);
	else if (num_digits<=100)
		return RandomLongNumberSpecificDigitsUniformUnitInterval<100>(number_to_make_random);
	else if (num_digits<=200)
		return RandomLongNumberSpecificDigitsUniformUnitInterval<200>(number_to_make_random);
	else if (num_digits<=400)
		return RandomLongNumberSpecificDigitsUniformUnitInterval<400>(number_to_make_random);
	else if (num_digits<=800)
		return RandomLongNumberSpecificDigitsUniformUnitInterval<800>(number_to_make_random);
	else if (num_digits<=1600)
		return RandomLongNumberSpecificDigitsUniformUnitInterval<1600>(number_to_make_random);
	else if (num_digits<=3200)
		return RandomLongNumberSpecificDigitsUniformUnitInterval<3200>(number_to_make_random);
	else if (num_digits<=6400)
		return RandomLongNumberSpecificDigitsUniformUnitInterval<6400>(number_to_make_random);
	else
		throw std::out_of_range("requesting random long number, longer than provided by RandomLongNumber(d)");
}





namespace bertini {
	
	/**
	 \class the fundamental floating point class for bertini 2.
	 
	 
	 */
	class Number{
		
		typedef boost::multiprecision::mpfr_float mp;
		
	private:
		
		std::unique_ptr<mp> number_as_multiple_precision_;
		std::unique_ptr<double> number_as_double_;
		unsigned int current_precision_;
		
	public:
		
		
		
		
		/**
		 default constructor creates an empty AMP number
		 */
		Number() : current_precision_(0)
		{
			//nothing else to put here.  leave empty.
		}
		
		
		/**
		 construct a Number from a string.  make it a multiple precision, with precision equal to the current default precision.
		 */
		Number(std::string new_value) : current_precision_(0)
		{
			
			if (new_value.size()==0) {
				//already have precision 0
				return;
			}
			
			
			else {
				number_as_multiple_precision_.reset(new mp(new_value)); // this may fail if the string is not a valid number.  we let boost handle this fault.
				current_precision_ = number_as_multiple_precision_->precision();
			}
		}
		
		
		/**
		 construct a Number from a string.  if it's short, make it a double.  otherwise, make it a multiple precision, with precision equal to the current default precision.
		 */
		Number(std::string new_value, unsigned int desired_precision) : current_precision_(desired_precision)
		{
			unsigned int cached_precision = mp::default_precision();
			mp::default_precision(desired_precision);
			number_as_multiple_precision_.reset(new mp(new_value));// this may fail if the string is not a valid number.  we let boost handle this fault.
			mp::default_precision(cached_precision);
		}
		
		
		
		/**
		 create a Number from a double.
		 */
		Number(double d) : current_precision_(16)
		{
			number_as_double_.reset(new double(d));
			// the multiple precision field is left empty
		}
		
		
		
		/**
		 create a Number from a boost::multiprecision::mpfr_float.
		 */
		Number(const boost::multiprecision::mpfr_float & m) : current_precision_(m.precision())
		{
			number_as_multiple_precision_.reset(new mp(m));
			// the multiple precision field is left empty
		}
		
		
		
		
		/**
		 copy constructor
		 */
		Number(const Number & new_value) : current_precision_(0)
		{
			this->ChangePrecision(new_value.Precision());
			if (current_precision_==0) {
				return;
			}
			else if (current_precision_<17) {
				*number_as_double_ = *(new_value.number_as_double_);
			}
			else{
				std::cout << number_as_multiple_precision_->precision() << std::endl;
				*number_as_multiple_precision_ = *(new_value.number_as_multiple_precision_);
				std::cout << number_as_multiple_precision_->precision() << std::endl;
			}
		}
		
		
		
		
		
		
		
		friend void swap(Number & left, Number & right)
		{
			std::swap(left.current_precision_, right.current_precision_);
			std::swap(left.number_as_double_, right.number_as_double_);
			std::swap(left.number_as_multiple_precision_, right.number_as_multiple_precision_);
		}
		
		/**
		 the assignment operator
		 
		 this uses the copy and swap idiom.
		 
		 \return the result of the assignment
		 \param other Another Number from which to assign
		 */
		
		//TODO: this copy uses the default precision, so it is impossible to
		Number& operator = (Number other)
		{
			std::cout << "other precision " << other.Precision() << " ";
			if (other.Precision()<=16) {
				std::cout << *other.number_as_double_ <<std::endl;
			}
			else
			{
				std::cout << other.number_as_multiple_precision_->precision() <<std::endl;
			}
			
			swap(*this,other);
			
			return *this;
		}
		
		
		
		/**
		 move constructor
		 
		 using the copy and swap idiom.
		 */
		Number(Number && other) : Number()
		{
			swap(*this, other);
		}
		
		
		
		
		
		/**
		 the assignment operator
		 
		 \return the result of the assignment
		 \param other a double from which to assign
		 */
		Number& operator = (double other)
		{
			ChangePrecision(16);
			*number_as_double_ = other;
			
			return *this;
		}
		
		
		
		/**
		 the assignment operator
		 
		 \return the result of the assignment
		 \param other a boost::multiprecision::mpfr_float from which to assign
		 */
		Number& operator = (mp other)
		{
			ChangePrecision(other.precision());
			*number_as_multiple_precision_ = other;
			return *this;
		}
		
		
		
		
		/**
		 \brief Change the precision of a bertini::Number to that which you desire.
		 
		 \param new_precision The new precision, in bits, of the number.  if 0, will become empty number.  Otherwise, sets the field accordingly.
		 */
		void ChangePrecision(unsigned int new_precision)
		{
			
			if (this->Precision()==0) {
				if (new_precision==0) {
					return;
				}
				else{
					if (new_precision<17) {
						number_as_double_.reset(new double);
					}
					else{
						number_as_multiple_precision_.reset(new mp);
						number_as_multiple_precision_->precision(new_precision);
					}
				}
			}
			else
			{// if pre-existing precision is not 0; already set to something.
				if (new_precision==0) {
					if (this->Precision()<17) {
						number_as_double_.release();
					}
					else{
						number_as_multiple_precision_.release();
					}
				}
				else{
					if (this->Precision()<17) {
						if (new_precision<17) {
							return;
						}
						else{
							number_as_multiple_precision_.reset(new mp);
							number_as_multiple_precision_->precision(new_precision);
							*number_as_multiple_precision_ = *number_as_double_;
							number_as_double_.release();
						}
					}
					else{
						if (new_precision<17) {
							number_as_double_.reset(new double(*number_as_multiple_precision_));
							number_as_multiple_precision_.release();
						}
						else{
							//simply change the precision, as already mp, just not at same precision.
							number_as_multiple_precision_->precision(new_precision);
						}
					}
				} // re: if (else) new_precision==0
			}//re: if Precision==0
			 //done manually changing the fields to correct precision, now change the tracked number representing the precision.
			current_precision_ = new_precision;
		}
		
		
		/**
		 Get the current precision of the number, in digits.
		 
		 \return the current number of DIGITS of the number
		 */
		unsigned int Precision() const
		{
			return current_precision_;
		}
		
		
		
		/**
		 turn the number into a randomly distributed number in [0,1).
		 */
		void MakeRandom()
		{
			if (Precision()==0) {
				throw std::runtime_error("trying to make a precision-0 number random.");
				return;
			}
			else if (Precision()<17) {
				*number_as_double_ = double(rand());
			}
			else{
				RandomLongNumberUniformUnitInterval(Precision(),*number_as_multiple_precision_);
			}
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		/***********************
		 **
		 **  comparitors
		 **
		 **********************/
		
		/**
		 test for equality.
		 */
		bool operator==(const Number & other) const
		{
			if (this->Precision()!= other.Precision()) {
				return false;
			}
			else
			{ // the two precisions are the same
				if (Precision()==0) {
					return true;
				}
				else if (Precision()<17) {
					if ( *(this->number_as_double_) == *(other.number_as_double_) ) { // dereference the pointers to compare the values
						return true;
					}
					else{
						return false;
					}
				}
				else {
					if ( *(this->number_as_multiple_precision_) == *(other.number_as_multiple_precision_) ) { // dereference the pointers to compare the values
						return true;
					}
					else{
						return false;
					}
				}
			}
		}
		
		
		bool operator==(double other) const
		{
			if (this->Precision()!= 16) {
				return false;
			}
			else
			{ // the two precisions are the same
				return (*number_as_double_==other);
			}
		}
		
		friend bool operator==(double lhs, const Number & rhs)
		{
			return rhs==lhs;
		}
		
		
		bool operator==(const mp & other) const
		{
			if (this->Precision()!= other.precision()) {
				return false;
			}
			else
			{ // the two precisions are the same
				return (*number_as_multiple_precision_==other);
			}
		}
		
		friend bool operator==(const mp & lhs, const Number & rhs)
		{
			return (rhs==lhs);
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		/******************
		 *
		 *  I/O streams
		 *
		 ****************/
		friend std::ostream & operator<<(std::ostream & out, const Number & printme)
		{
			if (printme.Precision()==0) {
				return out;
			}
			else if (printme.Precision()<17) {
				out << *printme.number_as_double_;
			}
			else{
				out << *printme.number_as_multiple_precision_;
			}
			
			return out;
		}
		
		
		
		friend std::istream & operator>>(std::istream & in_stream, Number & writeontome)
		{
			if (writeontome.Precision()==0) {
				throw std::runtime_error("trying to push onto an uninitialized bertini::Number");
			}
			else if (writeontome.Precision()<=16) {
				in_stream >> *(writeontome.number_as_double_);
			}
			else{
				in_stream >> *writeontome.number_as_multiple_precision_;
			}
			return in_stream;
		}
		
		
		
		
		
		
		
		/*******************
		 **
		 **   basic arithmetic operations and operators
		 **
		 *****************/
		
		
		/**
		 plain ol' addition
		 */
		Number operator+(const Number & other)
		{
			if (this->Precision()==0) {
				throw std::runtime_error("trying to add two bertini::Numbers and the left has 0 precision");
			}
			if (other.Precision()==0) {
				throw std::runtime_error("trying to add two bertini::Numbers and the right has 0 precision");
			}
			
			
			if (this->Precision()<17 && other.Precision()<17){
				// both double, so add the double.
				return bertini::Number( *(this->number_as_double_) + *(other.number_as_double_) );
			}
			else if (this->Precision()<17 && other.Precision()>=17){
				return bertini::Number( *(this->number_as_double_) + double(*(other.number_as_multiple_precision_)) );
			}
			else if (this->Precision()>=17 && other.Precision()<17){
				return bertini::Number( double(*(this->number_as_multiple_precision_)) + *(other.number_as_double_) );
			}
			else{
				bertini::Number result("0.0",std::min(this->Precision(),other.Precision()));
				*result.number_as_multiple_precision_ = *(this->number_as_multiple_precision_) + *(other.number_as_multiple_precision_);
				return result;
			}
		}
		
		
		
		Number operator+(double other) const
		{
			if (this->Precision()==0) {
				throw std::runtime_error("trying to add two bertini::Numbers and the left has 0 precision");
			}
			else if (this->Precision()<17){
				// both double, so add the double.
				return bertini::Number( *(this->number_as_double_) + other );
			}
			else{
				return bertini::Number( *number_as_multiple_precision_ + other);
			}
		}
		
		friend Number operator+(double lhs, const Number & rhs)
		{
			return rhs+lhs;
		}
		
		
		Number operator+(const mp & other) const
		{
			if (this->Precision()==0) {
				throw std::runtime_error("trying to add two bertini::Numbers and the left has 0 precision");
			}
			else if (this->Precision()<17){
				// both double, so add the double.
				return bertini::Number( *(this->number_as_double_) + other );
			}
			else{
				return bertini::Number( *number_as_multiple_precision_ + other);
			}
		}
		
		friend Number operator+(const mp & lhs, const Number & rhs)
		{
			return rhs+lhs;
		}
		
		
		
		
		
		
		/**
		 plain ol' subtraction
		 */
		Number operator-(const Number & other)
		{
			if (this->Precision()==0) {
				throw std::runtime_error("trying to subtract two bertini::Numbers and the left has 0 precision");
			}
			if (other.Precision()==0) {
				throw std::runtime_error("trying to subtract two bertini::Numbers and the right has 0 precision");
			}
			
			if (this->Precision()<17 && other.Precision()<17){
				// both double, so add the double.
				return bertini::Number( *(this->number_as_double_) - *(other.number_as_double_) );
			}
			else if (this->Precision()<17 && other.Precision()>=17){
				return bertini::Number( *(this->number_as_double_) - double(*(other.number_as_multiple_precision_)) );
			}
			else if (this->Precision()>=17 && other.Precision()<17){
				return bertini::Number( double(*(this->number_as_multiple_precision_)) - *(other.number_as_double_) );
			}
			else{
				bertini::Number result("0.0",std::min(this->Precision(),other.Precision()));
				*result.number_as_multiple_precision_ = *(this->number_as_multiple_precision_) - *(other.number_as_multiple_precision_);
				return result;
			}
		}
		
		
		
		
		/**
		 plain ol' negation
		 */
		Number operator-()
		{
			if (this->Precision()==0) {
				throw std::runtime_error("trying to negate a bertini::Numbers and it has 0 precision");
			}
			
			if (this->Precision()<17)
			{
				return bertini::Number(-*(this->number_as_double_));
			}
			else{
				return bertini::Number(-*(this->number_as_multiple_precision_));
			}
		}
		
		
		
		
		
		
		/**
		 plain ol' multiplication
		 */
		Number operator*(const Number & other)
		{
			if (this->Precision()==0) {
				throw std::runtime_error("trying to multiply two bertini::Numbers and the left has 0 precision");
			}
			if (other.Precision()==0) {
				throw std::runtime_error("trying to multiply two bertini::Numbers and the right has 0 precision");
			}
			
			if (this->Precision()<17 && other.Precision()<17){
				// both double, so add the double.
				return bertini::Number( *(this->number_as_double_) * *(other.number_as_double_) );
			}
			else if (this->Precision()<17 && other.Precision()>=17){
				return bertini::Number( *(this->number_as_double_) * double(*(other.number_as_multiple_precision_)) );
			}
			else if (this->Precision()>=17 && other.Precision()<17){
				return bertini::Number( double(*(this->number_as_multiple_precision_)) * *(other.number_as_double_) );
			}
			else{
				bertini::Number result("0.0",std::min(this->Precision(),other.Precision()));
				*result.number_as_multiple_precision_ = *(this->number_as_multiple_precision_) * *(other.number_as_multiple_precision_);
				return result;
			}
		}
		
		
		
		
		
		/**
		 plain ol' division
		 */
		Number operator/(const Number & other)
		{
			if (this->Precision()==0) {
				throw std::runtime_error("trying to divide two bertini::Numbers and the left has 0 precision");
			}
			if (other.Precision()==0) {
				throw std::runtime_error("trying to divide two bertini::Numbers and the right has 0 precision");
			}
			
			if (this->Precision()<17 && other.Precision()<17){
				// both double, so add the double.
				return bertini::Number( *(this->number_as_double_) / *(other.number_as_double_) );
			}
			else if (this->Precision()<17 && other.Precision()>=17){
				return bertini::Number( *(this->number_as_double_) / double(*(other.number_as_multiple_precision_)) );
			}
			else if (this->Precision()>=17 && other.Precision()<17){
				return bertini::Number( double(*(this->number_as_multiple_precision_)) / *(other.number_as_double_) );
			}
			else{
				bertini::Number result("0.0",std::min(this->Precision(),other.Precision()));
				*result.number_as_multiple_precision_ = *(this->number_as_multiple_precision_) / *(other.number_as_multiple_precision_);
				return result;
			}
		}
		
		
		
		
		
		/******************
		 **
		 **  non-elementary math operations
		 **
		 *******************/
		
		
		/**
		 support for the absolute value function
		 */
		friend Number abs(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take the absolute value of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( fabs(*(other.number_as_double_)) );
			}
			else{
				return Number( fabs(*(other.number_as_multiple_precision_)) );
			}
		}
		
		
		
		
		/**
		 support for the square root function
		 */
		friend Number sqrt(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take the square root of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( sqrt(*(other.number_as_double_)) );
			}
			else{
				std::cout << other.number_as_multiple_precision_->precision() << std::endl;
				return Number( sqrt(*(other.number_as_multiple_precision_)) );
			}
		}
		
		
		/**
		 support for the floor function
		 */
		friend Number floor(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take the floor of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( floor(*(other.number_as_double_)) );
			}
			else{
				return Number( floor(*(other.number_as_multiple_precision_)) );
			}
		}
		
		
		/**
		 support for the ceiling function
		 */
		friend Number ceil(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take the ceiling of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( ceil(*(other.number_as_double_)) );
			}
			else{
				return Number( ceil(*(other.number_as_multiple_precision_)) );
			}
		}
		
		
		/**
		 support for the exponential function
		 */
		friend Number exp(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take exp of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( exp(*(other.number_as_double_)) );
			}
			else{
				return Number( exp(*(other.number_as_multiple_precision_)) );
			}
		}
		
		
		/**
		 support for the log base e function
		 */
		friend Number log(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take log of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( log(*(other.number_as_double_)) );
			}
			else{
				return Number( log(*(other.number_as_multiple_precision_)) );
			}
		}
		
		/**
		 support for the log base 10 function
		 */
		friend Number log10(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take log10 of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( log10(*(other.number_as_double_)) );
			}
			else{
				return Number( log10(*(other.number_as_multiple_precision_)) );
			}
		}
		
		/**
		 support for the cosine function
		 */
		friend Number cos(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take cosine of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( cos(*(other.number_as_double_)) );
			}
			else{
				return Number( cos(*(other.number_as_multiple_precision_)) );
			}
		}
		
		/**
		 support for the sine function
		 */
		friend Number sin(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take sine of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( sin(*(other.number_as_double_)) );
			}
			else{
				return Number( sin(*(other.number_as_multiple_precision_)) );
			}
		}
		
		
		/**
		 support for the tangent function
		 */
		friend Number tan(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take tangent of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( tan(*(other.number_as_double_)) );
			}
			else{
				return Number( tan(*(other.number_as_multiple_precision_)) );
			}
		}
		
		/**
		 support for the arccosine function
		 */
		friend Number acos(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take arccosine of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( acos(*(other.number_as_double_)) );
			}
			else{
				return Number( acos(*(other.number_as_multiple_precision_)) );
			}
		}
		
		/**
		 support for the arcsine function
		 */
		friend Number asin(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take arcsine of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( asin(*(other.number_as_double_)) );
			}
			else{
				return Number( asin(*(other.number_as_multiple_precision_)) );
			}
		}
		
		
		/**
		 support for the arctangent function
		 */
		friend Number atan(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take arctangent of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( atan(*(other.number_as_double_)) );
			}
			else{
				return Number( atan(*(other.number_as_multiple_precision_)) );
			}
		}
		
		
		/**
		 support for the hyperbolic cosine function
		 */
		friend Number cosh(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take hyperbolic cosine of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( cosh(*(other.number_as_double_)) );
			}
			else{
				return Number( cosh(*(other.number_as_multiple_precision_)) );
			}
		}
		
		
		/**
		 support for the hyperbolic sine function
		 */
		friend Number sinh(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take hyperbolic sine of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( sinh(*(other.number_as_double_)) );
			}
			else{
				return Number( sinh(*(other.number_as_multiple_precision_)) );
			}
		}
		
		
		
		/**
		 support for the hyperbolic tangent function
		 */
		friend Number tanh(const Number & other)
		{
			if (other.Precision()==0) {
				throw std::runtime_error("trying to take hyperbolic tangent of a bertini::Number, and it has 0 precision");
			}
			else if (other.Precision()<17){
				return Number( tanh(*(other.number_as_double_)) );
			}
			else{
				return Number( tanh(*(other.number_as_multiple_precision_)) );
			}
		}
		
		
		
		/**
		 support for the std pow function.
		 */
		friend Number pow(const Number & base, const Number & exponent)
		{
			if (base.Precision()==0) {
				throw std::runtime_error("trying to take pow of a bertini::Number, and base has 0 precision");
			}
			
			if (exponent.Precision()==0) {
				throw std::runtime_error("trying to take pow of a bertini::Number, and exponent has 0 precision");
			}
			
			if (base.Precision()<17 && exponent.Precision()<17){
				return Number( pow(*base.number_as_double_,*exponent.number_as_double_) );
			}
			else if (base.Precision()<17 && exponent.Precision()>=17){
				return Number( pow(*base.number_as_double_,*exponent.number_as_multiple_precision_) );
			}
			else if (base.Precision()>=17 && exponent.Precision()<17){
				return Number( pow(*base.number_as_multiple_precision_,*exponent.number_as_double_) );
			}
			else{
				return Number( pow(*base.number_as_multiple_precision_,*exponent.number_as_multiple_precision_) );
			}
		}
		
		
		
		
		/**
		 support for atan2 operation
		 */
		friend Number atan2(const Number & left, const Number & right)
		{
			if (left.Precision()==0) {
				throw std::runtime_error("trying to take atan2 of a bertini::Number, and left has 0 precision");
			}
			
			if (right.Precision()==0) {
				throw std::runtime_error("trying to take atan2 of a bertini::Number, and right has 0 precision");
			}
			
			if (left.Precision()<17 && right.Precision()<17){
				return Number( atan2(*left.number_as_double_,*right.number_as_double_) );
			}
			else if (left.Precision()<17 && right.Precision()>=17){
				return Number( atan2(*left.number_as_double_,*right.number_as_multiple_precision_) );
			}
			else if (left.Precision()>=17 && right.Precision()<17){
				return Number( atan2(*left.number_as_multiple_precision_,*right.number_as_double_) );
			}
			else{
				return Number( atan2(*left.number_as_multiple_precision_,*right.number_as_multiple_precision_) );
			}
		}
		
		
		
		
		
		
		
		
		
		/*******************
		 **
		 **   conversion operators
		 **
		 *****************/
		 
		 
		 
		
		/**
		 convert the number to a double.
		 
		 returns nan is precision is 0.
		 */
		operator double() const
		{
			if (Precision()==0){
				return nan("");
			}
			else if (Precision()<=16){
				return *number_as_double_;
			}
			else{
				return double(*number_as_multiple_precision_);
			}
		}
		
		
		/**
		 convert Number to mpfr_float.
		 
		 returns nan if precision is 0.
		 */
		operator boost::multiprecision::mpfr_float() const
		{
			if (Precision()==0) {
				return mp(nan(""));
			}
			else if (Precision() <17){
				return mp(*number_as_double_);
			}
			else{
				return *number_as_multiple_precision_;
			}
			
		}
		
		
		
		/*****************
		 **
		 **   constants
		 **
		 ******************/
		//http://www.boost.org/doc/libs/1_57_0/libs/math/doc/html/math_toolkit/constants.html
		
		
		/**
		 get the number \f$\pi\f$, to default precision if the templated type is mp, and at double if you feed it a double
		 */
		template<typename T>
		static Number pi()
		{
			return Number(boost::math::constants::pi<T>());
		}
		
		/**
		 get the number \f$e\f$, to default precision if the templated type is mp, and at double if you feed it a double
		 */
		template<typename T>
		static Number e()
		{
			return Number(boost::math::constants::e<T>());
		}
		
	}; // re: class Number
	
	
	
} //re: bertini namespaces



#endif


