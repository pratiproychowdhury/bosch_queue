#include "bosch_queue.hpp"

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

class QueueTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(QueueTest);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testInsert);
    CPPUNIT_TEST(testPop);
    CPPUNIT_TEST(testPopTimeout);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override;
    void tearDown() override;
    void testConstructor();
    void testInsert();
    void testPop();
    void testPopTimeout();

private:
    Queue<int>* aQueue;
    int size;
};

CPPUNIT_TEST_SUITE_REGISTRATION(QueueTest);

void QueueTest::setUp() {

    size = 2;
    aQueue = new Queue<int>(size);
}

void QueueTest::tearDown() {

    delete aQueue;
}

void QueueTest::testConstructor() {

    CPPUNIT_ASSERT_EQUAL(size, aQueue->Size());
    CPPUNIT_ASSERT_EQUAL(aQueue->Count(), 0);
}

void QueueTest::testInsert() {

    int value(1);
    CPPUNIT_ASSERT_EQUAL(aQueue->Count(), 0);
    aQueue->Push(value);
    CPPUNIT_ASSERT_EQUAL(aQueue->Count(), 1);
}

void QueueTest::testPop() {

    int value(2);
    aQueue->Push(value);
    CPPUNIT_ASSERT_EQUAL(aQueue->Count(), 1);
    int temp = aQueue->Pop();
    CPPUNIT_ASSERT_EQUAL(temp, value);
    CPPUNIT_ASSERT_EQUAL(aQueue->Count(), 0);
}

void QueueTest::testPopTimeout() {

    int value(3);
    const int timeout(500);
    aQueue->Push(value);
    CPPUNIT_ASSERT_EQUAL(aQueue->Count(), 1);
    int temp = aQueue->PopWithTimeout(timeout);
    CPPUNIT_ASSERT_EQUAL( temp, value );
    CPPUNIT_ASSERT_EQUAL(aQueue->Count(), 0);
}


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( QueueTest, "test_queue_cppunit" );

CppUnit::Test *suite() {

    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    registry.registerFactory(&CppUnit::TestFactoryRegistry::getRegistry("test_queue_cppunit"));
    return registry.makeTest();
}

int main(int argc, char* argv[]) {

    std::string testPath = (argc > 1) ? std::string(argv[1]) : "";
    CppUnit::TestResult controller;

    CppUnit::TestResultCollector result;
    controller.addListener(&result);

    CppUnit::BriefTestProgressListener progress;
    controller.addListener(&progress);

    CppUnit::TestRunner runner;
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    runner.addTest(suite());
    try {
        std::cout  << std::endl;
        std::cout << "test Queue class with cppunit" << std::endl;
        std::cout << "========================="  << std::endl;
        runner.run( controller, testPath );
        std::cout << "=========================" << std::endl;
        std::cout << std::endl;

        CppUnit::CompilerOutputter outputter(&result, std::cerr);

        outputter.write();
    } catch (std::invalid_argument& e) {
        std::cerr  <<  std::endl
                   <<  "ERROR: "  <<  e.what()
                   << std::endl;
        return 0;
    }
    return result.wasSuccessful() ? 0 : 1;
}