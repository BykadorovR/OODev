#include <gtest/gtest.h>
#include "Interfaces.h"
#include "Auth.h"
#include "Geometry.h"
#include "DB.h"
#include "Map.h"
#include "Queue.h"

void fill_path(bezier_path* path, int count, int x, int y) {
	std::vector<bezier_line *> lines;
	for (int i = 0; i < count; i++) {
		Vector2f* dots[4];
		for (int j = 0; j < 4; j++) {
			dots[j] = new Vector2f(x, y);
		}
		lines.push_back(new bezier_line(*dots));
	}
	path->setLines(lines);
}


int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(AUTH, login_valid) {
	IDB* db; //Create valid DB object when it will be added
	Auth* auth = new Auth(db, "roman", "1234");
	ASSERT_TRUE(auth->login());
}

TEST(AUTH, login_no_valid) {
	IDB* db; //Create valid DB object when it will be added
	Auth* auth = new Auth(db, "null", "null");
	ASSERT_FALSE(auth->login());
}

TEST(AUTH, perm_valid) {
	IDB* db; //Create valid DB object when it will be added
	Auth* auth = new Auth(db, "roman", "1234");
	int perm = auth->permission();
	ASSERT_NE(-1, perm);
}

TEST(AUTH, perm_no_valid) {
	IDB* db; //Create valid DB object when it will be added
	Auth* auth = new Auth(db, "null", "null");
	int perm = auth->permission();
	ASSERT_EQ(-1, perm);
}

TEST(BIZIER, valid_path) {
	bezier_path* path = new bezier_path();
	fill_path(path, 5, 0,0);
	ASSERT_TRUE(path->test());
}

TEST(BIZIER, null) {
	bezier_path* path = new bezier_path();
	std::vector<bezier_line *> lines;
	path->setLines(lines);
	ASSERT_FALSE(path->test());
}

TEST(DATABASE, connect) {
	DB* db = new DB();
	ASSERT_TRUE(db->connect());
}

TEST(DATABASE, request) {
	DB* db = new DB();
	ASSERT_TRUE(db->connect());
}

TEST(MAP, get) {
	our::Map* map = new our::Map();
	bezier_path* path = new bezier_path();
	bezier_path* path1 = new bezier_path();
	bezier_path* path2 = new bezier_path();
	fill_path(path, 3, 0,0);
	fill_path(path1, 3, 1,1);
	fill_path(path2, 3, 2,2);
	map->add_path(path);
	map->add_path(path1);
	map->add_path(path2);
	std::vector<const bezier_path*> summary = map->get_paths();
	ASSERT_EQ(3, summary.size());
}

TEST(MAP, size) {
	our::Map* map = new our::Map();
	bezier_path* path = new bezier_path();
	bezier_path* path1 = new bezier_path();
	bezier_path* path2 = new bezier_path();
	fill_path(path, 3, 0,0);
	fill_path(path1, 3,2,2);
	fill_path(path2, 3,1,1);
	map->add_path(path);
	map->add_path(path1);
	map->add_path(path2);
	map->remove_path(1);
	std::vector<const bezier_path*> summary = map->get_paths();
	ASSERT_EQ(2, summary.size());
}

TEST(MAP, correctness_after_delete) {
	our::Map* map = new our::Map();
	bezier_path* path = new bezier_path();
	bezier_path* path1 = new bezier_path();
	bezier_path* path2 = new bezier_path();
	fill_path(path, 3, 0, 0);
	fill_path(path1, 3, 1, 1);
	fill_path(path2, 3, 2,2);
	map->add_path(path);
	map->add_path(path1);
	map->add_path(path2);
	map->remove_path(1);
	std::vector<const bezier_path*> summary = map->get_paths();
	ASSERT_EQ(summary[1]->m_lines[0]->get()[0](1, 0), 2);
	ASSERT_EQ(summary[1]->m_lines[0]->get()[0](0, 0), 2);
}

TEST(MAP, wrong_remove_negative) {
	our::Map* map = new our::Map();
	bezier_path* path = new bezier_path();
	bezier_path* path1 = new bezier_path();
	bezier_path* path2 = new bezier_path();
	fill_path(path, 3, 0, 0);
	fill_path(path1, 3, 1, 1);
	fill_path(path2, 3, 2, 2);
	map->add_path(path);
	map->add_path(path1);
	map->add_path(path2);
	ASSERT_FALSE(map->remove_path(-1));
}

TEST(MAP, wrong_remove_overflow) {
	our::Map* map = new our::Map();
	bezier_path* path = new bezier_path();
	bezier_path* path1 = new bezier_path();
	bezier_path* path2 = new bezier_path();
	fill_path(path, 3, 0, 0);
	fill_path(path1, 3, 1, 1);
	fill_path(path2, 3, 2, 2);
	map->add_path(path);
	map->add_path(path1);
	map->add_path(path2);
	ASSERT_FALSE(map->remove_path(3));
}

TEST(MAP, right_remove) {
	our::Map* map = new our::Map();
	bezier_path* path = new bezier_path();
	bezier_path* path1 = new bezier_path();
	bezier_path* path2 = new bezier_path();
	fill_path(path, 3, 0, 0);
	fill_path(path1, 3, 1, 1);
	fill_path(path2, 3, 2, 2);
	map->add_path(path);
	map->add_path(path1);
	map->add_path(path2);
	ASSERT_TRUE(map->remove_path(2));
}

class TestQueue : public ::testing::Test
{
protected:
	void SetUp()
	{
		DB* db = new DB();
		qu = new Queue((IDB*)db);
		/*
		Just check algorithm part in this fixture
		*/
		bezier_path* path = new bezier_path();
		bezier_path* path1 = new bezier_path();
		bezier_path* path2 = new bezier_path();
		fill_path(path, 3, 0, 0);
		fill_path(path1, 3, 1, 1);
		fill_path(path2, 3, 2, 2);
		qu->m_paths.push_back(path);
		qu->m_paths.push_back(path1);
		qu->m_paths.push_back(path2);
	}
	void TearDown()
	{
		
	}
	Queue* qu;
	DB* db;
};


TEST_F(TestQueue, accept_id) {
	auto test_path = qu->get_paths();
	qu->accept(0);
	ASSERT_EQ(test_path.size(), 2);
	ASSERT_EQ(test_path[0]->m_lines[0]->get()[0](1, 0), 1);
}

TEST_F(TestQueue, accept_path) {
	auto test_path = qu->get_paths();
	qu->accept(test_path[0]);
	ASSERT_EQ(test_path.size(), 2);
	ASSERT_EQ(test_path[0]->m_lines[0]->get()[0](1, 0), 1);
}

TEST_F(TestQueue, accept_no_valid_id) {
	auto test_path = qu->get_paths();
	ASSERT_FALSE(qu->accept(-1));
}

TEST_F(TestQueue, accept_no_valid_path) {
	auto test_path = qu->get_paths();
	ASSERT_FALSE(qu->accept(NULL));
}

TEST_F(TestQueue, reject_id) {
	auto test_path = qu->get_paths();
	qu->reject(0);
	ASSERT_EQ(test_path.size(), 2);
	ASSERT_EQ(test_path[0]->m_lines[0]->get()[0](1, 0), 1);
}

TEST_F(TestQueue, reject_path) {
	auto test_path = qu->get_paths();
	qu->reject(test_path[0]);
	ASSERT_EQ(test_path.size(), 2);
	ASSERT_EQ(test_path[0]->m_lines[0]->get()[0](1, 0), 1);
}

TEST_F(TestQueue, reject_no_valid_id) {
	auto test_path = qu->get_paths();
	ASSERT_FALSE(qu->reject(-1));
}

TEST_F(TestQueue, reject_no_valid_path) {
	auto test_path = qu->get_paths();
	ASSERT_FALSE(qu->reject(NULL));
}

TEST(Queue, accept_real_by_id_if_exist) {
	DB* db = new DB();
	Queue* qu = new Queue((IDB*)db);
	auto test_path = qu->get_paths();
	int size_before = test_path.size();
	if (size_before > 0) {
		qu->accept(0);
		ASSERT_EQ(size_before - 1, qu->get_paths().size());
	}	
}

TEST(Queue, reject_real_by_id_if_exist) {
	DB* db = new DB();
	Queue* qu = new Queue((IDB*)db);
	auto test_path = qu->get_paths();
	int size_before = test_path.size();
	if (size_before > 0) {
		qu->reject(0);
		ASSERT_EQ(size_before - 1, qu->get_paths().size());
	}
}

TEST(Reconnaissance, find_region_valid) {
	Reconnaissance* rec = new Reconnaissance();
	/*
	----------- x
	|0 1 2 3 4 5
	|6 7 8 9 ...      - regions
	|
	y
	*/

	Vector2f vec(1,0);
	ASSERT_EQ(1, rec->find_region(vec));
}

TEST(Reconnaissance, find_region_empty) {
	Reconnaissance* rec = new Reconnaissance();
	Vector2f vec;
	ASSERT_EQ(-1, rec->find_region(vec));
}

TEST(SharedMap, server_add_path) {
	DB* db = new DB();
	SharedMap* sm = new SharedMap((IDB *) db);
/* temporary	*/
	bezier_path* path = new bezier_path();
	fill_path(path, 3, 0, 0);
	sm->m_paths.push_back(path);

	auto before = sm->get_paths();
	sm->add_path(path);
	auto after = sm->get_paths();
	ASSERT_EQ(before.size(), after.size() - 1);
	ASSERT_EQ(after[after.size() - 1]->m_lines[0]->get()[0](1, 0), 0);
}

TEST(SharedMap, server_remove_path_valid) {
	DB* db = new DB();
	SharedMap* sm = new SharedMap((IDB *)db);
/* temporary	*/
	bezier_path* path = new bezier_path();
	fill_path(path, 3, 0, 0);
	sm->m_paths.push_back(path);

	auto before = sm->get_paths();
	auto elem = before[0];

	sm->remove_path(0);
	auto after = sm->get_paths();
	ASSERT_EQ(before.size(), after.size() + 1);
	ASSERT_NE(elem->m_lines[0]->get()[0](1, 0), after[0]->m_lines[0]->get()[0](1, 0));
	ASSERT_NE(elem->m_lines[0]->get()[0](0, 0), after[0]->m_lines[0]->get()[0](0, 0));
}

TEST(SharedMap, server_remove_path_invalid) {
	DB* db = new DB();
	SharedMap* sm = new SharedMap((IDB *)db);
	sm->remove_path(-1);
}


