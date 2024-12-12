#include "SimpleTestFramework.h"
#include "managers/ComponentManager.h"
#include "managers/EntityManager.h"

EntityManager& EM = EntityManager::getInstance();

TEST_CASE(TestCreate) {
    Entity entity;
    for (int i = 0; i < 10000; i++) {
        entity = EM.createEntity();
        ASSERT_EQUAL(entity, i << 16); // versipn 1
    }
}

TEST_CASE(TestDestroy) {
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(EM.isEntityAlive(i << 16)); // alive
        EM.destroyEntity(i << 16); // destroy entities
        ASSERT_TRUE(!EM.isEntityAlive(i << 16)); // not alive
    }
}

TEST_CASE(TestReuse) {
    Entity entity;
    for (int i = 0; i < 10; i++) {
        entity = EM.createEntity();
        ASSERT_EQUAL(entity, (i << 16) + 1); // version 2
    }
    entity = EM.createEntity();
    ASSERT_EQUAL(entity, (10000 << 16)); // goes back to where version 1 left off
}   
    

int main() {
    TestFramework::getInstance().runAllTests();
    return 0;
}