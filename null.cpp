#include "Shared2.hpp"  // toolbox::shared_ptr用
#include <memory>      // std::shared_ptr用
#include <iostream>
#include <cassert>
#include <string>

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

void testNullPointerBehavior() {
    std::cout << "\n=== NULL ポインタの挙動テスト ===\n" << std::endl;
    
    // テスト1: コンストラクタでのNULL
    {
        std::cout << "--- テスト1: コンストラクタでのNULL ---" << std::endl;
        
        std::shared_ptr<TestObject> stdNull(NULL);
        toolbox::shared_ptr<TestObject> toolboxNull(NULL);
        
        std::cout << "std::shared_ptr(NULL).use_count(): " << stdNull.use_count() << std::endl;
        std::cout << "toolbox::shared_ptr(NULL).use_count(): " << toolboxNull.use_count() << std::endl;
        
        // 操作確認
        std::cout << "std::shared_ptr は NULL? " << (stdNull ? "いいえ" : "はい") << std::endl;
        std::cout << "toolbox::shared_ptr は NULL? " << (toolboxNull ? "いいえ" : "はい") << std::endl;
    }
    
    // テスト2: コピーコンストラクタでのNULL
    {
        std::cout << "\n--- テスト2: コピーコンストラクタでのNULL ---" << std::endl;
        
        std::shared_ptr<TestObject> stdNull1(NULL);
        std::shared_ptr<TestObject> stdNull2(stdNull1);
        
        toolbox::shared_ptr<TestObject> toolboxNull1(NULL);
        toolbox::shared_ptr<TestObject> toolboxNull2(toolboxNull1);
        
        std::cout << "コピー後 std::shared_ptr.use_count(): " << stdNull1.use_count() << std::endl;
        std::cout << "コピー後 toolbox::shared_ptr.use_count(): " << toolboxNull1.use_count() << std::endl;
    }
    
    // テスト3: 通常ポインタからNULLへの代入
    {
        std::cout << "\n--- テスト3: 通常ポインタからNULLへの代入 ---" << std::endl;
        
        std::shared_ptr<TestObject> stdPtr(new TestObject(1));
        toolbox::shared_ptr<TestObject> toolboxPtr(new TestObject(2));
        
        std::cout << "代入前 std::shared_ptr.use_count(): " << stdPtr.use_count() << std::endl;
        std::cout << "代入前 toolbox::shared_ptr.use_count(): " << toolboxPtr.use_count() << std::endl;
        
        // NULLポインタを持つshared_ptrを作成
        std::shared_ptr<TestObject> stdNull(NULL);
        toolbox::shared_ptr<TestObject> toolboxNull(NULL);
        
        // 代入
        stdPtr = stdNull;
        toolboxPtr = toolboxNull;
        
        std::cout << "代入後 std::shared_ptr.use_count(): " << stdPtr.use_count() << std::endl;
        std::cout << "代入後 toolbox::shared_ptr.use_count(): " << toolboxPtr.use_count() << std::endl;
        
        // 操作確認
        std::cout << "代入後 std::shared_ptr は NULL? " << (stdPtr ? "いいえ" : "はい") << std::endl;
        std::cout << "代入後 toolbox::shared_ptr は NULL? " << (toolboxPtr ? "いいえ" : "はい") << std::endl;
    }
    
    // テスト4: resetメソッドでNULL
    {
        std::cout << "\n--- テスト4: resetメソッドでNULL ---" << std::endl;
        
        std::shared_ptr<TestObject> stdPtr(new TestObject(3));
        toolbox::shared_ptr<TestObject> toolboxPtr(new TestObject(4));
        
        std::cout << "reset前 std::shared_ptr.use_count(): " << stdPtr.use_count() << std::endl;
        std::cout << "reset前 toolbox::shared_ptr.use_count(): " << toolboxPtr.use_count() << std::endl;
        
        // NULLでリセット
        stdPtr.reset();
        toolboxPtr.reset();
        
        std::cout << "reset後 std::shared_ptr.use_count(): " << stdPtr.use_count() << std::endl;
        std::cout << "reset後 toolbox::shared_ptr.use_count(): " << toolboxPtr.use_count() << std::endl;
        
        // 操作確認
        std::cout << "reset後 std::shared_ptr は NULL? " << (stdPtr ? "いいえ" : "はい") << std::endl;
        std::cout << "reset後 toolbox::shared_ptr は NULL? " << (toolboxPtr ? "いいえ" : "はい") << std::endl;
    }
    
    // テスト5: NULLから通常ポインタへの代入
    {
        std::cout << "\n--- テスト5: NULLから通常ポインタへの代入 ---" << std::endl;
        
        std::shared_ptr<TestObject> stdPtr(NULL);
        toolbox::shared_ptr<TestObject> toolboxPtr(NULL);
        
        std::cout << "代入前 std::shared_ptr.use_count(): " << stdPtr.use_count() << std::endl;
        std::cout << "代入前 toolbox::shared_ptr.use_count(): " << toolboxPtr.use_count() << std::endl;
        
        // 通常ポインタを持つshared_ptrを作成
        std::shared_ptr<TestObject> stdNormal(new TestObject(5));
        toolbox::shared_ptr<TestObject> toolboxNormal(new TestObject(6));
        
        // 代入
        stdPtr = stdNormal;
        toolboxPtr = toolboxNormal;
        
        std::cout << "代入後 std::shared_ptr.use_count(): " << stdPtr.use_count() << std::endl;
        std::cout << "代入後 toolbox::shared_ptr.use_count(): " << toolboxPtr.use_count() << std::endl;
        
        // 値確認
        if (stdPtr) std::cout << "std::shared_ptr value: " << stdPtr->getValue() << std::endl;
        if (toolboxPtr) std::cout << "toolbox::shared_ptr value: " << toolboxPtr->getValue() << std::endl;
    }
    
    // テスト6: NULLからNULLへの代入
    {
        std::cout << "\n--- テスト6: NULLからNULLへの代入 ---" << std::endl;
        
        std::shared_ptr<TestObject> stdPtr1(NULL);
        std::shared_ptr<TestObject> stdPtr2(NULL);
        toolbox::shared_ptr<TestObject> toolboxPtr1(NULL);
        toolbox::shared_ptr<TestObject> toolboxPtr2(NULL);
        
        std::cout << "代入前 std::shared_ptr1.use_count(): " << stdPtr1.use_count() << std::endl;
        std::cout << "代入前 std::shared_ptr2.use_count(): " << stdPtr2.use_count() << std::endl;
        std::cout << "代入前 toolbox::shared_ptr1.use_count(): " << toolboxPtr1.use_count() << std::endl;
        std::cout << "代入前 toolbox::shared_ptr2.use_count(): " << toolboxPtr2.use_count() << std::endl;
        
        // 代入
        stdPtr1 = stdPtr2;
        toolboxPtr1 = toolboxPtr2;
        
        std::cout << "代入後 std::shared_ptr1.use_count(): " << stdPtr1.use_count() << std::endl;
        std::cout << "代入後 std::shared_ptr2.use_count(): " << stdPtr2.use_count() << std::endl;
        std::cout << "代入後 toolbox::shared_ptr1.use_count(): " << toolboxPtr1.use_count() << std::endl;
        std::cout << "代入後 toolbox::shared_ptr2.use_count(): " << toolboxPtr2.use_count() << std::endl;
    }
    
    std::cout << "\n=== テスト終了 ===\n" << std::endl;
    std::cout << "最終的なオブジェクト数: " << TestObject::getInstanceCount() << std::endl;
    assert(TestObject::getInstanceCount() == 0);
}

int main() {
    testNullPointerBehavior();
    return 0;
}