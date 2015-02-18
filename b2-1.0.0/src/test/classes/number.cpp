
#define private public
#include "classes/number.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Number

#include <exception>
#include <sstream>
#include <boost/test/unit_test.hpp>


// start of the test suite using bertini::Number
BOOST_FIXTURE_TEST_SUITE( number_basics, bertini::Number )



/**
 an empty constructed bertini::Number should have 0 precision, and two nullptr unique_ptr's.
 */
BOOST_AUTO_TEST_CASE(empty_has_both_fields_null)
{
	BOOST_CHECK_EQUAL(current_precision_,0);
	BOOST_CHECK(number_as_double_==nullptr);
	BOOST_CHECK(number_as_multiple_precision_==nullptr);
}



BOOST_AUTO_TEST_CASE(change_precision_to_double_has_null_mp)
{
	ChangePrecision(16);
	
	BOOST_CHECK_EQUAL(current_precision_,16);
	BOOST_CHECK(number_as_double_!=nullptr);
	BOOST_CHECK(number_as_multiple_precision_==nullptr);
	
}


BOOST_AUTO_TEST_CASE(change_precision_to_mp_has_null_double50)
{
	ChangePrecision(50);
	
	BOOST_CHECK_EQUAL(current_precision_,50);
	BOOST_CHECK(number_as_double_==nullptr);
	BOOST_CHECK(number_as_multiple_precision_!=nullptr);
	BOOST_CHECK_EQUAL(number_as_multiple_precision_->precision(),50);
	
}



BOOST_AUTO_TEST_CASE(change_precision_to_mp_has_null_double100)
{
	ChangePrecision(100);
	
	BOOST_CHECK_EQUAL(current_precision_,100);
	BOOST_CHECK(number_as_double_==nullptr);
	BOOST_CHECK(number_as_multiple_precision_!=nullptr);
	BOOST_CHECK_EQUAL(number_as_multiple_precision_->precision(),100);
}



BOOST_AUTO_TEST_CASE(make_random_multiple_precision_correct_precision30)
{
	ChangePrecision(30);
	MakeRandom();
	BOOST_CHECK_EQUAL(number_as_multiple_precision_->precision(),30);
}




BOOST_AUTO_TEST_CASE(make_random_multiple_precision_correct_precision50)
{
	ChangePrecision(50);
	MakeRandom();
	BOOST_CHECK_EQUAL(number_as_multiple_precision_->precision(),50);
}

BOOST_AUTO_TEST_CASE(make_random_multiple_precision_correct_precision5000)
{
	ChangePrecision(5000);
	MakeRandom();
	BOOST_CHECK(number_as_multiple_precision_->precision()>=5000);
}

BOOST_AUTO_TEST_SUITE_END()
// the end of the suite using a bertini::Number as a fixture










/*******************
 **
 **  exercise constructors
 **
 *********************/



BOOST_AUTO_TEST_SUITE(Number)


BOOST_AUTO_TEST_CASE(number_constructor_from_number)
{
	
	boost::multiprecision::mpfr_float::default_precision(50);
	
	bertini::Number a("2.0",30);
	
	BOOST_CHECK_EQUAL(a.Precision(),30);
	
	bertini::Number b(a);
	
	BOOST_CHECK_EQUAL(b.Precision(),30);
	BOOST_CHECK_EQUAL(*a.number_as_multiple_precision_,*b.number_as_multiple_precision_);
	
}

/**
 changing the default precision in boost::multiprecision::mpfr_float actually changes that of a default constructed bertini::Number
 */
BOOST_AUTO_TEST_CASE(change_default_precision)
{
	boost::multiprecision::mpfr_float::default_precision(16);
	boost::multiprecision::mpfr_float a;
	BOOST_CHECK_EQUAL(a.precision(),16);
	
	bertini::Number a_bertini;
	
	
	boost::multiprecision::mpfr_float::default_precision(75);
	boost::multiprecision::mpfr_float b;
	BOOST_CHECK_EQUAL(b.precision(),75);
	
	
	boost::multiprecision::mpfr_float::default_precision(4962);
	boost::multiprecision::mpfr_float c;
	BOOST_CHECK_EQUAL(c.precision(),4962);
	
}




/**
 create a bertini::Number from a string, but the default is higher, and despite the fact you put in a short number, you get a higher-precision number, because that's the default.
 */
BOOST_AUTO_TEST_CASE(Number_create_from_string_multiple_precision0)
{
	boost::multiprecision::mpfr_float::default_precision(1000);
	bertini::Number a("0.1234");
	BOOST_CHECK_EQUAL(a.Precision(),1000);
}


/**
 make a number which is longer than default precision, yet it is constructed as the default precision.
 */
BOOST_AUTO_TEST_CASE(Number_create_from_string_multiple_precision1)
{
	boost::multiprecision::mpfr_float::default_precision(1000);
	
	bertini::Number a("0.12345678901234567890123456789");
	BOOST_CHECK_EQUAL(a.Precision(),1000);
}

/**
 precision of the desired string and the default match
 */
BOOST_AUTO_TEST_CASE(Number_create_from_string_multiple_precision2)
{
	boost::multiprecision::mpfr_float::default_precision(30);
	bertini::Number a("-0.10000000000000000000000000000");
	BOOST_CHECK_EQUAL(a.Precision(),30);
}


/**
 
 */
BOOST_AUTO_TEST_CASE(Number_create_from_string_multiple_precision3)
{
	boost::multiprecision::mpfr_float::default_precision(30);
	bertini::Number a("1e30");
	BOOST_CHECK_EQUAL(a.Precision(),30);
}


/**
 exercise the (string,digits10) constructor, for constructing a bertini::Number with precision different from the current default.
 */
BOOST_AUTO_TEST_CASE(Number_create_from_string_precision_different_from_default)
{
	boost::multiprecision::mpfr_float::default_precision(50);
	
	bertini::Number a("-0.10000000000000000000000000000",30);
	BOOST_CHECK_EQUAL(a.number_as_multiple_precision_->precision(),30);
	
}




/**
 round-trip (or convert-and-come-back) a bertini::Number into a string and back again, from one which was in double form.
 */
BOOST_AUTO_TEST_CASE(Number_round_tripping_number_double)
{
	srand(time(NULL));
	double burnone = double(rand());
	
	bertini::Number a;
	a.ChangePrecision(16);
	a.MakeRandom();
	
	std::stringstream converter;
	converter.precision(a.Precision());
	converter << a;
	bertini::Number b;  b.ChangePrecision(16);
	converter >> b;
	
	
	BOOST_CHECK_EQUAL(a,b);
}

/**
 round-trip (or convert-and-come-back) a bertini::Number into a string and back again, from one which was in multiple precision form.
 
 note that you have to have one more digit of precision on the stream for conversion, than in the bertini::Number itself.  I find this weird.  
 
 TODO: find out why this is.
 */
BOOST_AUTO_TEST_CASE(Number_round_tripping_number_multiple_precision)
{
	
	bertini::Number a;
	a.ChangePrecision(50);
	a.MakeRandom();
	
	std::stringstream converter;
	converter.precision(a.Precision()+1);
	converter << a;
	bertini::Number b;
	b.ChangePrecision(50);
	converter >> b;
	

	BOOST_CHECK_EQUAL(a,b);
}


/**
 find the absolute value of a bertini::Number which was in double form.
 */
BOOST_AUTO_TEST_CASE(Number_absolute_value_double)
{
	bertini::Number a(-0.1);
	
	BOOST_CHECK_EQUAL( double(abs(a)) , 0.1);
	
}


/**
 check the absolute value of a bertini::Number which was in multiple precision
 */
BOOST_AUTO_TEST_CASE(Number_absolute_value_multiple_precision)
{
	bertini::Number a("-0.10000000000000000000000000000");
	BOOST_CHECK_EQUAL( double(abs(a)) , 0.1);
	
}





BOOST_AUTO_TEST_CASE(Number_add_both_double_check_correct_precision)
{
	bertini::Number a(1.0), b(2.0);
	
	bertini::Number c = a+b;
	
	BOOST_CHECK_EQUAL(c.Precision(),16);
	BOOST_CHECK_EQUAL(*c.number_as_double_,3.0);
	
}



BOOST_AUTO_TEST_CASE(Number_add_mixed_double_and_multiple_precision_correct_precision)
{
	boost::multiprecision::mpfr_float::default_precision(50);
	bertini::Number a(0), b("1.2"), c;
	
	c = a+b;
	
	BOOST_CHECK_EQUAL(c.Precision(),16);
	BOOST_CHECK_EQUAL(*c.number_as_double_,1.2);
}


BOOST_AUTO_TEST_CASE(Number_add_both_multiple_precision_check_correct_precision)
{
	bertini::Number a, b, c;
	a.ChangePrecision(30);
	b.ChangePrecision(70);
	
	a.MakeRandom();
	b.MakeRandom();
	
	c = a+b;
	
	BOOST_CHECK_EQUAL(c.Precision(),30);
	BOOST_CHECK_EQUAL(c.number_as_multiple_precision_->precision(),30);
}

BOOST_AUTO_TEST_CASE(Number_add_throw_unset_double)
{
	bertini::Number a, b(2.0);
	BOOST_CHECK_THROW(a+b,std::exception);
	BOOST_CHECK_THROW(b+a,std::exception);
}

BOOST_AUTO_TEST_CASE(Number_add_throw_unset_multiple_precision)
{
	bertini::Number a, b("2.0",30);
	BOOST_CHECK_THROW(a+b,std::exception);
	BOOST_CHECK_THROW(b+a,std::exception);
}











BOOST_AUTO_TEST_CASE(Number_subtract_both_double_check_correct_precision)
{
	bertini::Number a(1.0), b(2.0);
	
	bertini::Number c = a-b;
	
	BOOST_CHECK_EQUAL(c.Precision(),16);
	BOOST_CHECK_EQUAL( (*c.number_as_double_),-1.0);
	
}



BOOST_AUTO_TEST_CASE(Number_subtract_mixed_double_and_multiple_precision_correct_precision)
{
	boost::multiprecision::mpfr_float::default_precision(50);
	bertini::Number a(0), b("1.2"), c;
	
	c = a-b;
	
	BOOST_CHECK_EQUAL(c.Precision(),16);
	BOOST_CHECK_EQUAL(*c.number_as_double_,-1.2);
}


BOOST_AUTO_TEST_CASE(Number_subtract_both_multiple_precision_check_correct_precision)
{
	bertini::Number a("1.323",30), b("2.43455",70), c;

	
	c = a-b;
	
	BOOST_CHECK_EQUAL(c.Precision(),30);
	BOOST_CHECK_EQUAL(c.number_as_multiple_precision_->precision(),30);
	
	boost::multiprecision::mpfr_float expected_result("-1.11155");
	expected_result.precision(30);
	
	BOOST_CHECK_EQUAL(*c.number_as_multiple_precision_,expected_result);
}

BOOST_AUTO_TEST_CASE(Number_subtract_throw_unset_double)
{
	bertini::Number a, b(2.0);
	BOOST_CHECK_THROW(a-b,std::exception);
	BOOST_CHECK_THROW(b-a,std::exception);
}

BOOST_AUTO_TEST_CASE(Number_subtract_throw_unset_multiple_precision)
{
	bertini::Number a, b("2.0",30);
	BOOST_CHECK_THROW(a-b,std::exception);
	BOOST_CHECK_THROW(b-a,std::exception);
}










BOOST_AUTO_TEST_CASE(Number_multiply_both_double_check_correct_precision)
{
	bertini::Number a(1.5), b(2.0);
	
	bertini::Number c = a*b;
	
	BOOST_CHECK_EQUAL(c.Precision(),16);
	BOOST_CHECK_EQUAL(*c.number_as_double_,3.0);
	
}



BOOST_AUTO_TEST_CASE(Number_multiply_mixed_double_and_multiple_precision_correct_precision)
{
	boost::multiprecision::mpfr_float::default_precision(50);
	bertini::Number a(1.33), b("1.2"), c;
	
	c = a*b;
	
	BOOST_CHECK_EQUAL(c.Precision(),16);
	BOOST_CHECK_EQUAL(*c.number_as_double_,1.596);
}


BOOST_AUTO_TEST_CASE(Number_multiply_both_multiple_precision_check_correct_precision)
{
	bertini::Number a("1.323",30), b("2.43455",70), c;
	
	
	c = a*b;
	
	BOOST_CHECK_EQUAL(c.Precision(),30);
	BOOST_CHECK_EQUAL(c.number_as_multiple_precision_->precision(),30);
	
	boost::multiprecision::mpfr_float expected_result("3.22090965");
	expected_result.precision(30);
	
	BOOST_CHECK(abs(double(abs(c))-expected_result)<1e-15);
}

BOOST_AUTO_TEST_CASE(Number_multiply_throw_unset_double)
{
	bertini::Number a, b(2.0);
	BOOST_CHECK_THROW(a*b,std::exception);
	BOOST_CHECK_THROW(b*a,std::exception);
}

BOOST_AUTO_TEST_CASE(Number_multiply_throw_unset_multiple_precision)
{
	bertini::Number a, b("2.0",30);
	BOOST_CHECK_THROW(a*b,std::exception);
	BOOST_CHECK_THROW(b*a,std::exception);
}








BOOST_AUTO_TEST_CASE(Number_divide_both_double_check_correct_precision)
{
	bertini::Number a(1.5), b(2.0);
	
	bertini::Number c = a/b;
	
	BOOST_CHECK_EQUAL(c.Precision(),16);
	BOOST_CHECK_EQUAL(*c.number_as_double_,0.75);
	
}



BOOST_AUTO_TEST_CASE(Number_divide_mixed_double_and_multiple_precision_correct_precision)
{
	boost::multiprecision::mpfr_float::default_precision(50);
	bertini::Number a(1.33), b("1.2"), c;
	
	c = a/b;
	
	BOOST_CHECK_EQUAL(c.Precision(),16);
	BOOST_CHECK_EQUAL(*c.number_as_double_,1.108333333333333333333333333333333333333);
}


BOOST_AUTO_TEST_CASE(Number_divide_both_multiple_precision_check_correct_precision)
{
	bertini::Number a("1.323",30), b("2.43455",70), c;
	
	
	c = a/b;
	
	BOOST_CHECK(c.Precision()==30);
	BOOST_CHECK(c.number_as_multiple_precision_->precision()==30);
	
	boost::multiprecision::mpfr_float expected_result("0.543426916678646977880922552422418927522");
	expected_result.precision(30);

	BOOST_CHECK( (abs(*c.number_as_multiple_precision_ - expected_result) < boost::multiprecision::mpfr_float("1e-30")) );
}

BOOST_AUTO_TEST_CASE(Number_divide_throw_unset_double)
{
	bertini::Number a, b(2.0);
	BOOST_CHECK_THROW(a/b,std::exception);
	BOOST_CHECK_THROW(b/a,std::exception);
}

BOOST_AUTO_TEST_CASE(Number_divide_throw_unset_multiple_precision)
{
	bertini::Number a, b("2.0",30);
	BOOST_CHECK_THROW(a/b,std::exception);
	BOOST_CHECK_THROW(b/a,std::exception);
}









BOOST_AUTO_TEST_CASE(Number_square_root)
{
	bertini::Number a(2.0);
	
	BOOST_CHECK_EQUAL( sqrt(a),double(1.41421356237309504880168872421) );
	
	bertini::Number c;
	BOOST_CHECK_THROW( a = sqrt(c) ,std::exception);
	
	boost::multiprecision::mpfr_float expected_result( "1.414213562373095048801688724209698078569671875376948073176679737990732478462107038850387534327641573")
													  ;
	expected_result.precision(30);
	bertini::Number b("2.0",30);
	
	BOOST_CHECK_EQUAL(b.Precision(),30);
	
	std::cout << "asdf" <<std::endl;
	bertini::Number d = sqrt(b);
	std::cout << "qwer" <<std::endl;
	BOOST_CHECK_EQUAL(d.Precision(),30);
	BOOST_CHECK_EQUAL(d.number_as_multiple_precision_->precision(),30);
	
	std::cout.precision(33);
	std::cout << d.Precision() << " " << d << std::endl;
	
	std::cout << expected_result.precision() << " " << expected_result << std::endl;
	
	BOOST_CHECK(abs( boost::multiprecision::mpfr_float(d)-expected_result)<1e-30);
	
	
}





BOOST_AUTO_TEST_CASE(Number_cosine_of_pi_is_minus_one)
{
	BOOST_CHECK_EQUAL(cos(bertini::Number::pi<boost::multiprecision::mpfr_float>()) ,boost::multiprecision::mpfr_float(-1));
	BOOST_CHECK_EQUAL(cos(bertini::Number::pi<double>()) ,double(-1));
}



BOOST_AUTO_TEST_SUITE_END()






