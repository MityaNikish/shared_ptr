#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DBG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define newDBG_NEW

#include "pch.h"
#include "shared.hpp"
#include "Exception.hpp"

class VideoBuffer 
{
private:
    int count;

public:
    VideoBuffer() : count(640 * 480) { }
    VideoBuffer(int count) : count(count) { }

    int get_count(void)
    {
        return count;
    }
};

//void main()
//{
//    _CrtDumpMemoryLeaks();
//}


TEST(TestCaseConstructorPtr, Constructor_1)
{
    SharedPTR<VideoBuffer> pImage(new VideoBuffer());
    EXPECT_EQ(pImage->get_count(), 640 * 480);
}

TEST(TestCaseConstructorPtr, Constructor_2)
{
    SharedPTR<VideoBuffer> pImage(new VideoBuffer);
    SharedPTR<VideoBuffer> pImage_copy(pImage);
    EXPECT_EQ(pImage_copy->get_count(), 640 * 480);
}

TEST(TestCaseConstructorPtr, Constructor_3)
{
    SharedPTR<VideoBuffer> pImage(new VideoBuffer);
    SharedPTR<VideoBuffer> pImage_copy(std::move(pImage));
    EXPECT_EQ(pImage_copy->get_count(), 640 * 480);
}


TEST(TestCaseConstructorPtr, ConstructorArr_1)
{
    SharedPTR<VideoBuffer[]> pArr(new VideoBuffer);

    EXPECT_EQ(pArr[0].get_count(), 640 * 480);
}

TEST(TestCaseConstructorPtr, ConstructorArr_2)
{
    VideoBuffer* arr = new VideoBuffer[]{ {320}, {480}, {720} };
    SharedPTR<VideoBuffer[]> pArr(arr);
    SharedPTR<VideoBuffer[]> pArr_copy(pArr);

    EXPECT_EQ(pArr_copy[0].get_count(), 320);
}

TEST(TestCaseConstructorPtr, ConstructorArr_3)
{
    VideoBuffer* arr = new VideoBuffer[]{ {320}, {480}, {720} };
    SharedPTR<VideoBuffer[]> pArr1(arr);
    SharedPTR<VideoBuffer[]> pArr(std::move(pArr1));

    EXPECT_EQ(pArr[0].get_count(), 320);
}

TEST(TestCaseOperator, Equals_1)
{
    //assignment
    SharedPTR<VideoBuffer> pImage_1(new VideoBuffer());
    SharedPTR<VideoBuffer> pImage_2(new VideoBuffer(192 * 144));
    pImage_1 = pImage_2;
   EXPECT_EQ(pImage_1->get_count(), 192 * 144);

   //re-assignment
   pImage_1 = pImage_2;
   EXPECT_EQ(pImage_1->get_count(), 192 * 144);
}

TEST(TestCaseOperator, Equals_2)
{
    //assignment
    VideoBuffer* obj = new VideoBuffer(192 * 144);
    SharedPTR<VideoBuffer> pImage_1(new VideoBuffer());
    pImage_1 = obj;
    EXPECT_EQ(pImage_1->get_count(), 192 * 144);

    //re-assignment
    pImage_1 = obj;
    EXPECT_EQ(pImage_1->get_count(), 192 * 144);

    //assignment nulptr
    pImage_1 = nullptr;
    EXPECT_EQ(pImage_1.get(), nullptr);
}


TEST(TestCaseOperator, EqualsArr_1)
{
    //assignment
    SharedPTR<VideoBuffer[]> pArr1(new VideoBuffer[]{ {1080} });
    SharedPTR<VideoBuffer[]> pArr2(new VideoBuffer[]{ {320}, {480}, {720} });
    pArr1 = pArr2;
    EXPECT_EQ(pArr1[0].get_count(), 320);

    //re-assignment
    pArr1 = pArr2;
    EXPECT_EQ(pArr1[0].get_count(), 320);
}

TEST(TestCaseOperator, EqualsArr_2)
{
    //assignment
    VideoBuffer* obj = new VideoBuffer[]{ {1080} };
    SharedPTR<VideoBuffer[]> pArr(new VideoBuffer[]{ {320}, {480}, {720} });
    pArr = obj;
    EXPECT_EQ(pArr[0].get_count(), 1080);

    //re-assignment
    pArr = obj;
    EXPECT_EQ(pArr[0].get_count(), 1080);

    //assignment nulptr
    pArr = nullptr;
    EXPECT_EQ(pArr.get(), nullptr);
}


TEST(TestCaseOperator, Dereference_1)
{
    SharedPTR<VideoBuffer> pImage(new VideoBuffer(192 * 144));

    EXPECT_EQ(pImage->get_count(), 192 * 144);

    EXPECT_EQ((*pImage).get_count(), 192 * 144);

    EXPECT_EQ(pImage.get()->get_count(), 192 * 144);
}


TEST(TestCaseMethods, Swap)
{
    SharedPTR<VideoBuffer[]> pArr1(new VideoBuffer[]{ {320}, {480}, {720} });
    SharedPTR<VideoBuffer[]> pArr2(new VideoBuffer[]{ {1080}, {1440} });

    pArr1.swap(pArr2);

    EXPECT_EQ(pArr1[0].get_count(), 1080);
    EXPECT_EQ(pArr2[0].get_count(), 320);
}

TEST(TestCaseMethods, SwapArr)
{
    SharedPTR<VideoBuffer> pImage_1(new VideoBuffer(640 * 480));
    SharedPTR<VideoBuffer> pImage_2(new VideoBuffer(192 * 144));

    pImage_1.swap(pImage_2);

    EXPECT_EQ(pImage_1->get_count(), 192 * 144);
    EXPECT_EQ(pImage_2->get_count(), 640 * 480);
}

TEST(TestCaseMethods, Release)
{
    SharedPTR<VideoBuffer> pImage(new VideoBuffer(640 * 480));

    pImage.release();

    EXPECT_EQ(pImage.get(), nullptr);
}

TEST(TestCaseMethods, ReleaseArr)
{
    SharedPTR<VideoBuffer[]> pArr(new VideoBuffer[]{ {320}, {480}, {720} });

    pArr.release();

    EXPECT_EQ(pArr.get(), nullptr);
}

TEST(TestCaseMethods, Reset)
{
    VideoBuffer* obj = new VideoBuffer(192 * 144);
    SharedPTR<VideoBuffer> pImage(new VideoBuffer(640 * 480));

    //
    pImage.reset(obj);
    EXPECT_EQ(pImage->get_count(), 192 * 144);

    //
    pImage.reset();
    EXPECT_EQ(pImage.get(), nullptr);
}

TEST(TestCaseMethods, ResetArr)
{
    VideoBuffer* obj = new VideoBuffer[]{ {320}, {480}, {720} };
    SharedPTR<VideoBuffer[]> pArr(obj);

    //
    pArr.reset(obj);
    EXPECT_EQ(pArr[0].get_count(), 320);

    //
    pArr.reset();
    EXPECT_EQ(pArr.get(), nullptr);
}
TEST(TestCaseVector, Vector_Int)
{
    std::vector<int> *vec1 = new std::vector<int>;
    *vec1 = {12, 58, 88};
    std::vector<int>* vec2 = new std::vector<int>;
    *vec2 = { 1, 5, 8 };
    SharedPTR<std::vector<int>> pVec(vec1);

    pVec = vec2;

}


TEST(TestCaseExeption, Test1)
{
    SharedPTR<VideoBuffer> pImage(new VideoBuffer());
    pImage.reset();

    EXPECT_THROW({ pImage->get_count(); }, Invalid_value_exception);
}

TEST(TestCaseExeption, Test2)
{
    SharedPTR<VideoBuffer> pImage(new VideoBuffer());
    pImage.reset();

    EXPECT_THROW({ (*pImage).get_count(); }, Invalid_value_exception);
}


