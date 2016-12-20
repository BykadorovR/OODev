#include <gtest/gtest.h>
#include "Interfaces.h"
#include "Auth.h"
#include "Geometry.h"
#define TESTING 1
#include "Map.h"
#include "Queue.h"

using namespace newmeteo;


std::vector<bezier_line *> fill_path(int count, int x, int y) {
	std::vector<bezier_line *> lines;
	for (int i = 0; i < count; i++) {
		Vector2f* dots;
		dots = new Vector2f[4];
		for (int j = 0; j < 4; j++) {
			dots[j] = Vector2f(x, y);
		}
		lines.push_back(new bezier_line(dots));
	}
	return lines;
}


/*int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	std::getchar(); // keep console window open until Return keystroke
}
*/
TEST(AUTH, login_valid) {
	IDB* db; //Create valid DB object when it will be added
	Auth* auth = new Auth(db);
	ASSERT_TRUE(auth->login("roman", "1234"));
}

TEST(AUTH, login_no_valid) {
	IDB* db; //Create valid DB object when it will be added
	Auth* auth = new Auth(db);
	ASSERT_FALSE(auth->login("null", "null"));
}

TEST(AUTH, perm_valid) {
	IDB* db; //Create valid DB object when it will be added
	Auth* auth = new Auth(db);
	auth->login("roman", "1234");
	int perm = auth->permission();
	ASSERT_NE(-1, perm);
}

TEST(AUTH, perm_no_valid) {
	IDB* db; //Create valid DB object when it will be added
	Auth* auth = new Auth(db);
	auth->login("null", "null");
	int perm = auth->permission();
	ASSERT_EQ(-1, perm);
}

TEST(BIZIER, valid_path) {
	bezier_path* path = new bezier_path(fill_path(5, 0, 0));
	ASSERT_TRUE(path->test());
}

TEST(BIZIER, null) {
	std::vector<bezier_line *> lines;
	bezier_path* path = new bezier_path(lines);
	ASSERT_FALSE(path->test());
}

TEST(DATABASE, connect) {
	//DB* db = new DB();
	//ASSERT_TRUE(db->connect());
}

TEST(DATABASE, request) {
	//DB* db = new DB();
	//ASSERT_TRUE(db->connect());
}

TEST(MAP, get) {
	newmeteo::Map* map = new newmeteo::Map();
	bezier_path* path = new bezier_path(fill_path(3, 0, 0));
	bezier_path* path1 = new bezier_path(fill_path(3, 1, 1));
	bezier_path* path2 = new bezier_path(fill_path(3, 2, 2));
	map->add_path(path);
	map->add_path(path1);
	map->add_path(path2);
	auto summary = map->get_paths();
	ASSERT_EQ(3, summary.size());
}

TEST(MAP, size) {
	newmeteo::Map* map = new newmeteo::Map();
	bezier_path* path = new bezier_path(fill_path(3, 0, 0));
	bezier_path* path1 = new bezier_path(fill_path(3, 1, 1));
	bezier_path* path2 = new bezier_path(fill_path(3, 2, 2));
	map->add_path(path);
	map->add_path(path1);
	map->add_path(path2);
	map->remove_path(1);
	auto summary = map->get_paths();
	ASSERT_EQ(2, summary.size());
}

TEST(MAP, correctness_after_delete) {
	newmeteo::Map* map = new newmeteo::Map();
	bezier_path* path = new bezier_path(fill_path(3, 0, 0));
	bezier_path* path1 = new bezier_path(fill_path(3, 1, 1));
	bezier_path* path2 = new bezier_path(fill_path(3, 2, 2));

	map->add_path(path);
	map->add_path(path1);
	map->add_path(path2);
	map->remove_path(1);
	auto summary = map->get_paths();
	int index = 0;
	for (auto it = summary.begin(); it != summary.end(); it++) {	
		if (index == 1) {			
			ASSERT_EQ((*it)->get_lines()[0]->get()[0](1, 0), 2);
			ASSERT_EQ((*it)->get_lines()[0]->get()[0](0, 0), 2);
		}
		index++;
	}
}

TEST(MAP, wrong_remove_negative) {
	newmeteo::Map* map = new newmeteo::Map();
	bezier_path* path = new bezier_path(fill_path(3, 0, 0));
	bezier_path* path1 = new bezier_path(fill_path(3, 1, 1));
	bezier_path* path2 = new bezier_path(fill_path(3, 2, 2));

	map->add_path(path);
	map->add_path(path1);
	map->add_path(path2);
	ASSERT_FALSE(map->remove_path(-1));
}

TEST(MAP, wrong_remove_overflow) {
	newmeteo::Map* map = new newmeteo::Map();
	bezier_path* path = new bezier_path(fill_path(3, 0, 0));
	bezier_path* path1 = new bezier_path(fill_path(3, 1, 1));
	bezier_path* path2 = new bezier_path(fill_path(3, 2, 2));

	map->add_path(path);
	map->add_path(path1);
	map->add_path(path2);
	ASSERT_FALSE(map->remove_path(3));
}

TEST(MAP, right_remove) {
	newmeteo::Map* map = new newmeteo::Map();
	bezier_path* path = new bezier_path(fill_path(3, 0, 0));
	bezier_path* path1 = new bezier_path(fill_path(3, 1, 1));
	bezier_path* path2 = new bezier_path(fill_path(3, 2, 2));

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
		accepted = new Queue((IDB*)db);
		rejected = new Queue((IDB*)db);
		bezier_path* path = new bezier_path(fill_path(3, 0, 0));
		bezier_path* path1 = new bezier_path(fill_path(3, 1, 1));
		bezier_path* path2 = new bezier_path(fill_path(3, 2, 2));

		accepted->m_added_paths.push_back(path);
		accepted->m_added_paths.push_back(path1);
		accepted->m_added_paths.push_back(path2);
	}
	void TearDown()
	{
		
	}
	Queue* accepted;
	Queue* rejected;
	IDB* db;
};


TEST_F(TestQueue, accept_id) {
	auto test_path = accepted->get_added_paths();
	accepted->accept(0);
	ASSERT_EQ(test_path.size(), 2);
	int index = 0;
	for (auto it = test_path.begin(); it != test_path.end(); it++) {
		if (index == 0) {			
			ASSERT_EQ((*it)->get_lines()[0]->get()[0](1, 0), 1);
		}
		index++;
	}
}

TEST_F(TestQueue, accept_no_valid_id) {
	ASSERT_FALSE(accepted->accept(-1));
}

TEST_F(TestQueue, reject_id) {
	auto test_path = accepted->get_removed_paths();
	accepted->reject(0);
	ASSERT_EQ(test_path.size(), 2);
	int index = 0;
	for (auto it = test_path.begin(); it != test_path.end(); it++) {
		if (index == 0) {
			ASSERT_EQ((*it)->get_lines()[0]->get()[0](1, 0), 1);
		}
		index++;
	}
}

TEST_F(TestQueue, reject_no_valid_id) {
	ASSERT_FALSE(accepted->reject(-1));
}

TEST(Queue, accept_real_by_id_if_exist) {
	IDB* db;// = new DB();
	Queue* qu = new Queue((IDB*)db);
	auto test_path = qu->get_added_paths();
	int size_before = test_path.size();
	if (size_before > 0) {
		qu->accept(0);
		ASSERT_EQ(size_before - 1, qu->get_added_paths().size());
	}	
}

TEST(Queue, reject_real_by_id_if_exist) {
	IDB* db;// = new DB();
	Queue* qu = new Queue((IDB*)db);
	auto test_path = qu->get_added_paths();
	int size_before = test_path.size();
	if (size_before > 0) {
		qu->reject(0);
		ASSERT_EQ(size_before - 1, qu->get_added_paths().size());
	}
}

TEST(Reconnaissance, find_region_valid) {
	//Висит
	Reconnaissance* rec = new Reconnaissance();
	Vector2f* dots;
	dots = new Vector2f[4];
	for (int j = 0; j < 4; j++) {
		dots[j] = Vector2f(j, j + 1);
	}
	std::vector<unsigned int> regions;
	rec->find_region(new bezier_line(dots), regions);
	ASSERT_EQ(1, regions.size());
	ASSERT_EQ(2147483648, regions[0]);
}

TEST(Reconnaissance, find_path_invalid) {
	//Висит
	Reconnaissance* rec = new Reconnaissance();
	bezier_path* path = new bezier_path(fill_path(3, 0, 1));
	auto lines = path->get_lines();
	std::vector<unsigned int> regions;
	rec->find_region(path, regions);
	std::cout << regions.size() << std::endl;
	//Проверить, что регион корректный
	//EXPECT_THAT(rec->find_region(path, regions), HasSubstr(""));
}

TEST(Reconnaissance, find_region_empty) {
	Reconnaissance* rec = new Reconnaissance();
	bezier_line* line = NULL;
	std::vector<unsigned int> regions;
	rec->find_region(line, regions);
	ASSERT_EQ(0, regions.size());
}
/*
TEST(SharedMap, server_add_path) {
	DB* db = new DB();
	SharedMap* sm = new SharedMap((IDB *) db);
// temporary	
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
// temporary	
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
*/

