#include <cassert>
#include <iostream>
#include <memory>  // std::shared_ptr用
#include <string>

#include "Shared.hpp"  // toolbox::shared_ptr用

// テスト用のクラス
class TestObject {
 public:
  TestObject(int val = 0) : value(val) {
    std::cout << "TestObject constructor: " << value << std::endl;
    instances++;
  }

  ~TestObject() {
    std::cout << "TestObject destructor: " << value << std::endl;
    instances--;
  }

  void setValue(int val) { value = val; }
  int getValue() const { return value; }

  static int getInstanceCount() { return instances; }

 private:
  int value;
  static int instances;
};

int TestObject::instances = 0;

// 両方の共有ポインタの挙動を比較するテスト関数
void runTests() {
  std::cout << "=== テスト開始 ===" << std::endl;

  // 基本的な構築とアクセス
  {
    std::cout << "\n--- テスト1: 基本的な構築とアクセス ---" << std::endl;

    std::shared_ptr<TestObject> stdPtr(new TestObject(1));
    toolbox::shared_ptr<TestObject> toolboxPtr(new TestObject(2));
    // toolbox::shared_ptr<TestObject> ptr;

    std::cout << "std::shared_ptr value: " << stdPtr->getValue() << std::endl;
    std::cout << "toolbox::shared_ptr value: " << toolboxPtr->getValue()
              << std::endl;

    assert(stdPtr.use_count() == 1);
    assert(toolboxPtr.use_count() == 1);

    // operator* のテスト
    (*stdPtr).setValue(10);
    (*toolboxPtr).setValue(20);

    assert(stdPtr->getValue() == 10);
    assert(toolboxPtr->getValue() == 20);
  }
  std::cout << "テスト1終了時のオブジェクト数: "
            << TestObject::getInstanceCount() << std::endl;

  // コピーと参照カウント
  {
    std::cout << "\n--- テスト2: コピーと参照カウント ---" << std::endl;

    std::shared_ptr<TestObject> stdPtr1(new TestObject(3));
    toolbox::shared_ptr<TestObject> toolboxPtr1(new TestObject(4));

    {
      std::shared_ptr<TestObject> stdPtr2 = stdPtr1;
      toolbox::shared_ptr<TestObject> toolboxPtr2 = toolboxPtr1;

      std::cout << "std::shared_ptr count: " << stdPtr1.use_count()
                << std::endl;
      std::cout << "toolbox::shared_ptr count: " << toolboxPtr1.use_count()
                << std::endl;

      assert(stdPtr1.use_count() == 2);
      assert(toolboxPtr1.use_count() == 2);

      // 値の変更が共有されることを確認
      stdPtr2->setValue(30);
      toolboxPtr2->setValue(40);

      assert(stdPtr1->getValue() == 30);
      assert(toolboxPtr1->getValue() == 40);
    }

    // スコープを抜けた後のカウント
    std::cout << "スコープ抜けた後のカウント:" << std::endl;
    std::cout << "std::shared_ptr count: " << stdPtr1.use_count() << std::endl;
    std::cout << "toolbox::shared_ptr count: " << toolboxPtr1.use_count()
              << std::endl;

    assert(stdPtr1.use_count() == 1);
    assert(toolboxPtr1.use_count() == 1);
  }
  std::cout << "テスト2終了時のオブジェクト数: "
            << TestObject::getInstanceCount() << std::endl;

  // リセット機能
  {
    std::cout << "\n--- テスト3: リセット機能 ---" << std::endl;

    std::shared_ptr<TestObject> stdPtr(new TestObject(5));
    toolbox::shared_ptr<TestObject> toolboxPtr(new TestObject(6));

    // 新しいオブジェクトでリセット
    stdPtr.reset(new TestObject(50));
    toolboxPtr.reset(new TestObject(60));

    std::cout << "リセット後の値:" << std::endl;
    std::cout << "std::shared_ptr value: " << stdPtr->getValue() << std::endl;
    std::cout << "toolbox::shared_ptr value: " << toolboxPtr->getValue()
              << std::endl;

    std::cout << "std::shared_ptr count: " << stdPtr.use_count() << std::endl;
    std::cout << "toolbox::shared_ptr count: " << toolboxPtr.use_count()
              << std::endl;

    assert(stdPtr->getValue() == 50);
    assert(toolboxPtr->getValue() == 60);

    // NULLでリセット
    stdPtr.reset();
    toolboxPtr.reset();

    std::cout << "NULLリセット後:" << std::endl;
    std::cout << "std::shared_ptr is " << (stdPtr ? "not null" : "null")
              << std::endl;
    std::cout << "toolbox::shared_ptr is " << (toolboxPtr ? "not null" : "null")
              << std::endl;

    assert(!stdPtr);
    assert(!toolboxPtr);
  }
  std::cout << "テスト3終了時のオブジェクト数: "
            << TestObject::getInstanceCount() << std::endl;

  // スワップ機能
  {
    std::cout << "\n--- テスト4: スワップ機能 ---" << std::endl;

    std::shared_ptr<TestObject> stdPtr1(new TestObject(7));
    std::shared_ptr<TestObject> stdPtr2(new TestObject(8));

    toolbox::shared_ptr<TestObject> toolboxPtr1(new TestObject(9));
    toolbox::shared_ptr<TestObject> toolboxPtr2(new TestObject(10));

    std::cout << "スワップ前:" << std::endl;
    std::cout << "stdPtr1: " << stdPtr1->getValue()
              << ", stdPtr2: " << stdPtr2->getValue() << std::endl;
    std::cout << "toolboxPtr1: " << toolboxPtr1->getValue()
              << ", toolboxPtr2: " << toolboxPtr2->getValue() << std::endl;

    // スワップ
    std::swap(stdPtr1, stdPtr2);
    toolbox::swap(toolboxPtr1, toolboxPtr2);

    std::cout << "スワップ後:" << std::endl;
    std::cout << "stdPtr1: " << stdPtr1->getValue()
              << ", stdPtr2: " << stdPtr2->getValue() << std::endl;
    std::cout << "toolboxPtr1: " << toolboxPtr1->getValue()
              << ", toolboxPtr2: " << toolboxPtr2->getValue() << std::endl;

    std::cout << "std::shared_ptr count: " << stdPtr1.use_count() << std::endl;
    std::cout << "toolbox::shared_ptr count: " << toolboxPtr1.use_count()
              << std::endl;

    assert(stdPtr1->getValue() == 8);
    assert(stdPtr2->getValue() == 7);
    assert(toolboxPtr1->getValue() == 10);
    assert(toolboxPtr2->getValue() == 9);
  }
  std::cout << "テスト4終了時のオブジェクト数: "
            << TestObject::getInstanceCount() << std::endl;

  // 代入演算子
  {
    std::cout << "\n--- テスト5: 代入演算子 ---" << std::endl;

    std::shared_ptr<TestObject> stdPtr1(new TestObject(11));
    std::shared_ptr<TestObject> stdPtr2(new TestObject(12));

    toolbox::shared_ptr<TestObject> toolboxPtr1(new TestObject(13));
    toolbox::shared_ptr<TestObject> toolboxPtr2(new TestObject(14));

    // 代入前のカウント
    std::cout << "代入前のカウント:" << std::endl;
    std::cout << "stdPtr1 count: " << stdPtr1.use_count()
              << ", stdPtr2 count: " << stdPtr2.use_count() << std::endl;
    std::cout << "toolboxPtr1 count: " << toolboxPtr1.use_count()
              << ", toolboxPtr2 count: " << toolboxPtr2.use_count()
              << std::endl;

    // 代入
    stdPtr1 = stdPtr2;
    toolboxPtr1 = toolboxPtr2;

    // 代入後のカウント
    std::cout << "代入後のカウント:" << std::endl;
    std::cout << "stdPtr1 count: " << stdPtr1.use_count()
              << ", stdPtr2 count: " << stdPtr2.use_count() << std::endl;
    std::cout << "toolboxPtr1 count: " << toolboxPtr1.use_count()
              << ", toolboxPtr2 count: " << toolboxPtr2.use_count()
              << std::endl;

    assert(stdPtr1.use_count() == 2);
    assert(stdPtr2.use_count() == 2);
    assert(toolboxPtr1.use_count() == 2);
    assert(toolboxPtr2.use_count() == 2);

    // 値が同じであることを確認
    assert(stdPtr1->getValue() == 12);
    assert(toolboxPtr1->getValue() == 14);
  }
  std::cout << "テスト5終了時のオブジェクト数: "
            << TestObject::getInstanceCount() << std::endl;

  // 自己代入テスト
  {
    std::cout << "\n--- テスト6: 自己代入 ---" << std::endl;

    std::shared_ptr<TestObject> stdPtr(new TestObject(15));
    toolbox::shared_ptr<TestObject> toolboxPtr(new TestObject(16));

    // 代入前のカウント
    std::cout << "自己代入前のカウント:" << std::endl;
    std::cout << "stdPtr count: " << stdPtr.use_count() << std::endl;
    std::cout << "toolboxPtr count: " << toolboxPtr.use_count() << std::endl;

    // 自己代入
    stdPtr = stdPtr;
    toolboxPtr = toolboxPtr;

    // 代入後のカウント
    std::cout << "自己代入後のカウント:" << std::endl;
    std::cout << "stdPtr count: " << stdPtr.use_count() << std::endl;
    std::cout << "toolboxPtr count: " << toolboxPtr.use_count() << std::endl;

    assert(stdPtr.use_count() == 1);
    assert(toolboxPtr.use_count() == 1);

    // 値が変わっていないことを確認
    assert(stdPtr->getValue() == 15);
    assert(toolboxPtr->getValue() == 16);
  }
  std::cout << "テスト6終了時のオブジェクト数: "
            << TestObject::getInstanceCount() << std::endl;

  std::cout << "\n=== テスト終了 ===" << std::endl;
}

int main() {
  runTests();

  // 最終的なメモリリークがないか確認
  std::cout << "\n最終的なオブジェクト数: " << TestObject::getInstanceCount()
            << std::endl;
  assert(TestObject::getInstanceCount() == 0);

  return 0;
}