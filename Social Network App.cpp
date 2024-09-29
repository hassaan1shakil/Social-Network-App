#include<iostream>
#include<fstream>
using namespace std;

class Helper;
class Object;
class Users;
class Pages;
class Date;
class Posts;
class Comments;
class Activity;
class Memory;

class Facebook
{
private:

	Users* CurrentUser;
	Pages** PagesList;
	Users** UserList;
	Posts** PostsList;
	Comments** CommentsList;

	int TotalUsers;
	int TotalPages;
	int TotalPosts;
	int TotalComments;

public:

	Facebook();
	~Facebook();
	void Run();
	void LoadApp();
	void LoadPages(ifstream&);
	void LoadUsers(ifstream&);
	void LoadPosts(ifstream&);
	void LoadComments(ifstream&);
	void SetTotalUsers(int);
	void SetTotalPages(int);
	void SetTotalPosts(int);
	void SetTotalComments(int);
	int GetTotalPosts();
	int GetTotalComments();
	Pages* SearchPage(char*);
	Users* SearchUser(char*&);
	Object* SearchObject(char object[]);
	Posts* SearchPost(char*&);

	Users* SetCurrentUser(const char User[]);
	void ViewPostLikes(const char Post[]);
	void ViewPost(const char Post[]);
	void ViewPage(const char Page[]);

	void PostComment(const char Post[], const char text[]);
	void LikePost(const char Post[]);
	void ShareMemories(const char Post[], const char text[]);
};

class Object
{
	char* ObjectID;

public:

	Object();
	~Object();
	virtual void AddPostToTimeline(Posts*&) = 0;

	virtual void AddCommentor(Object*&);
	virtual void PrintFullName() = 0;
	virtual void PrintListView() = 0;

	void SetID(char*&);
	void PrintID();
	char* GetObjectID();
};

class Users : public Object
{
private:

	char* FirstName;
	char* LastName;
	Pages** LikedPages;
	Users** Friends;
	Posts** Timeline;

	int num_LikedPages;
	int num_friends;
	int num_Posts;

public:

	Users();
	~Users();
	void ReadUsers(ifstream&);
	void AssociatePages(Pages*&);
	void LikePage(Pages*&);
	void AssociateFriends(Users*&);
	Users* SearchUserByID(char*&);
	void AddFriend(Users*&);
	void AddPostToTimeline(Posts*&);
	void PrintFriends();
	void PrintPages();

	void PrintLatestPosts();
	void AccessTimeline();
	void PrintFullName();
	void PrintListView();

	void ViewFriendList();
	void ViewLikedPages();
	void ViewHome();
	void SeeMemories();
	void ShareMemories(Posts*&, char*&, int);
	void ViewTimeline();
};

class Pages : public Object
{
private:

	char* PageName;
	Posts** Timeline;

	int num_posts;
	int num_LikedPosts;

public:

	Pages();
	~Pages();
	void ReadPages(ifstream&);
	Pages* SearchPageByID(char*&);
	void AddPostToTimeline(Posts*&);
	void PrintPages();

	void AccessTimeline();
	void PrintFullName();
	void PrintListView();
	void PrintAllPosts();
};

class Date
{
	int Day;
	int Month;
	int Year;

public:

	static Date SystemDate;

	Date();
	void ReadDate(ifstream&);

	static void SetSystemDate(int day, int month, int year);
	bool CheckDate(int, int, int);
	void PrintDate();

	void SetDate(int, int, int);
	int CompareDateForMemory(Date&);
};

class Posts
{
protected:

	int PostType;
	char* PostID;
	Date date;
	Activity* activity;
	char* Text;
	Object* SharedBy;
	Object** LikedBy;
	Comments** comments;

	int no_Likes;
	int no_Comments;

public:

	Posts();
	Posts(Users*&, char*&);
	virtual ~Posts();
	void ReadPosts(ifstream&);
	void SetSharedBy(Object*&);
	void AddLikeToPost(Object*&);
	void AddComment(Comments*&);
	Posts* SearchPostByID(char*&);

	virtual void PrintPost(bool PrintDate = false);
	void LikePost(Object*);
	void PrintLikes();
	void AddNewComment(char*&, Users*&, int);
	void PrintPostDetails(bool PrintDate = false, bool PrintComments = true);
	void ShowMemories();

	void SetPostID(int);
};

class Comments
{
	char* CommentID;
	char* Text;
	Object* Commentor;

public:

	Comments();
	Comments(char*&, Object*, int);
	~Comments();
	void ReadComments(ifstream&);
	void AddCommentor(Object*&);

	void PrintComments();
	void ReadText(ifstream&);
	void SetCommentID(int);
};

class Activity
{
	int Type;
	char* Value;

public:

	Activity();
	~Activity();
	void ReadData(ifstream& PostsFile);

	void PrintActivity();
};

class Memory : public Posts
{
	Posts* OriginalPost;

public:

	Memory();
	Memory(Users*, char*&, Posts*&, int);
	~Memory();

	void PrintPost(bool);
};

class Helper
{
public:

	static char* GetStringFromBuffer(char* buffer);	
	static char* StringCopy(const char* source, char* dest);
	static int StringLength(const char* string);

	static char* IntToChar(int num);
	static char* GetStringFromConstLiteral(const char literal[]);
};

Date Date::SystemDate;

int main()
{
	Facebook App;
	App.Run();

	return 0;
}

// Controller Class

Facebook::Facebook()
{
	CurrentUser = 0;
	PagesList = 0;
	UserList = 0;
	PostsList = 0;
	CommentsList = 0;

	TotalUsers = 0;
	TotalPages = 0;
	TotalPosts = 0;
	TotalComments = 0;
}

Facebook::~Facebook()
{
	cout << "~Facebook() Called\n";

	for (int i = 0; i < TotalPages; i++)
	{
		if (PagesList[i])
			delete PagesList[i];
	}

	if (PagesList)
		delete[] PagesList;

	for (int i = 0; i < TotalUsers; i++)
	{
		if (UserList[i])
			delete UserList[i];
	}

	if (UserList)
		delete[] UserList;

	if (PostsList)
		delete[] PostsList;

	if (CommentsList)
		delete[] CommentsList;
}

void Facebook::Run()
{
	LoadApp();

	Date::SetSystemDate(15, 11, 2017);

	CurrentUser = SetCurrentUser("u4");		// User is set manually

	if (CurrentUser)
	{
		CurrentUser->ViewFriendList();
		CurrentUser->ViewLikedPages();
		CurrentUser->ViewHome();
		CurrentUser->ViewTimeline();

		ViewPostLikes("post5");
		LikePost("post5");
		ViewPostLikes("post5");

		ViewPost("post4");
		PostComment("post4", "Good Luck for your Result");
		ViewPost("post4");

		ViewPost("post8");
		PostComment("post8", "Thanks for the wishes");
		ViewPost("post8");

		ViewPage("p1");

		CurrentUser->SeeMemories();
		ShareMemories("post10", "Never thought I will be a specialist in this field.....");
		CurrentUser->ViewTimeline();
	}

	CurrentUser = SetCurrentUser("u11");

	if (CurrentUser)
	{
		CurrentUser->ViewHome();
		CurrentUser->ViewTimeline();
	}
}

void Facebook::LoadApp()
{
	ifstream PagesFile("SocialNetworkPages.txt"), UsersFile("SocialNetworkUsers.txt");
	ifstream PostsFile("SocialNetworkPosts.txt"), CommentsFile("SocialNetworkComments.txt");

	LoadPages(PagesFile);

	LoadUsers(UsersFile);

	LoadPosts(PostsFile);

	LoadComments(CommentsFile);
}

void Facebook::LoadPages(ifstream& PagesFile)
{
	if (PagesFile.is_open())
	{
		int totalPages;

		PagesFile >> totalPages;

		PagesList = new Pages * [totalPages + 1];

		for (int i = 0; i < totalPages; i++)
		{
			PagesList[i] = new Pages;
			PagesList[i]->ReadPages(PagesFile);
		}

		PagesList[totalPages] = 0;

		SetTotalPages(totalPages);
	}

	else
	{
		cout << "\nError Reading Pages From File\n\n";
	}

	PagesFile.close();
}

void Facebook::LoadUsers(ifstream& UsersFile)
{
	if (UsersFile.is_open())
	{
		int totalUsers;

		UsersFile >> totalUsers;
		SetTotalUsers(totalUsers);

		UserList = new Users * [totalUsers + 1];
		char*** temp_friends = new char** [totalUsers];

		char FriendID[4];
		FriendID[0] = ' ';

		char PageID[4];
		PageID[0] = ' ';
		Pages* PagePtr = 0;

		Users* FriendPtr = 0;

		for (int i = 0, j; i < totalUsers; i++)
		{
			UserList[i] = new Users;
			UserList[i]->ReadUsers(UsersFile);

			// Reading temp friends

			temp_friends[i] = new char* [10];

			for (j = 0; FriendID[0] != '-' && j < 10; j++)
			{
				UsersFile >> FriendID;
				temp_friends[i][j] = Helper::GetStringFromBuffer(FriendID);
			}

			FriendID[0] = '0';
			temp_friends[i][j] = NULL;

			// Association with Pages

			PageID[0] = ' ';
			PagePtr = 0;

			for (int k = 0; PageID[0] != '-' && k < 10; k++)
			{
				UsersFile >> PageID;
				PagePtr = SearchPage(PageID);

				if (PagePtr)
					UserList[i]->AssociatePages(PagePtr);
			}
		}

		// Association with Friends

		for (int i = 0; i < totalUsers; i++)
		{
			FriendPtr = 0;
			//FriendID = 0;

			for (int j = 0; temp_friends[i][j] != NULL; j++)
			{
				FriendPtr = Facebook::SearchUser(temp_friends[i][j]);
				UserList[i]->AssociateFriends(FriendPtr);
			}
		}

		// DeAllocation

		for (int i = 0; i < totalUsers; i++)
		{
			for (int j = 0; temp_friends[i][j] != NULL; j++)
			{
				if (temp_friends[i][j])
					delete[] temp_friends[i][j];
			}

			if (temp_friends[i])
				delete[] temp_friends[i];
		}

		if (temp_friends)
			delete[] temp_friends; temp_friends = 0;

		UserList[totalUsers] = 0;
	}

	else
	{
		cout << "\nError Reading Users From File\n\n";
	}

	UsersFile.close();
}

void Facebook::LoadPosts(ifstream& PostsFile)
{
	if (PostsFile.is_open())
	{
		int totalPosts;
		char SharerID[4];
		Object* Sharer = 0; Object* LikedBy = 0;

		PostsFile >> totalPosts;
		SetTotalPosts(totalPosts);

		PostsList = new Posts * [totalPosts + 1];
		char LikedID[4];

		for (int i = 0; i < totalPosts; i++)
		{
			PostsList[i] = new Posts;
			PostsList[i]->ReadPosts(PostsFile);

			PostsFile >> SharerID;
			Sharer = SearchObject(SharerID);
			PostsList[i]->SetSharedBy(Sharer);

			LikedID[0] = ' ';
			LikedBy = 0;

			for (int j = 0; LikedID[0] != '-'; j++)
			{
				PostsFile >> LikedID;
				LikedBy = SearchObject(LikedID);

				if (LikedBy)
					PostsList[i]->AddLikeToPost(LikedBy);
			}

			LikedID[0] = '0';

			if (Sharer)
				Sharer->AddPostToTimeline(PostsList[i]);
		}

		PostsList[totalPosts] = 0;
	}

	else
	{
		cout << "\nError Reading Posts From File\n\n";
	}

	PostsFile.close();
}

void Facebook::LoadComments(ifstream& CommentsFile)
{
	if (CommentsFile.is_open())
	{
		int totalComments;
		char temp[100], * PostID = 0, * ObjectID = 0;
		Object* object = 0; Posts* post = 0;

		CommentsFile >> totalComments;

		CommentsList = new Comments * [totalComments + 1];

		for (int i = 0; i < totalComments; i++)
		{
			CommentsList[i] = new Comments;
			CommentsList[i]->ReadComments(CommentsFile);

			// Association with Post

			CommentsFile >> temp;
			PostID = Helper::GetStringFromBuffer(temp);

			post = Facebook::SearchPost(PostID);

			if (post)
				post->AddComment(CommentsList[i]);

			// Association with User 

			CommentsFile >> temp;
			ObjectID = Helper::GetStringFromBuffer(temp);

			object = Facebook::SearchObject(ObjectID);

			if (object)
				CommentsList[i]->AddCommentor(object);

			CommentsList[i]->ReadText(CommentsFile);
		}

		if (PostID)
			delete[] PostID;

		if (ObjectID)
			delete[] ObjectID;

		CommentsList[totalComments] = 0;

		SetTotalComments(totalComments);
	}

	else
	{
		cout << "\nError Reading Comments From File\n\n";
	}

	CommentsFile.close();
}

void Facebook::SetTotalUsers(int users)
{
	TotalUsers = users;
}

void Facebook::SetTotalPages(int pages)
{
	TotalPages = pages;
}

void Facebook::SetTotalPosts(int posts)
{
	TotalPosts = posts;
}

void Facebook::SetTotalComments(int comments)
{
	TotalComments = comments;
}

int Facebook::GetTotalPosts()
{
	return TotalPosts;
}

int Facebook::GetTotalComments()
{
	return TotalComments;
}

Pages* Facebook::SearchPage(char* tempID)
{
	Pages* PageID = 0;

	for (int i = 0; i < TotalPages; i++)
	{
		PageID = PagesList[i]->SearchPageByID(tempID);

		if (PageID)
			return PageID;
	}

	return 0;
}

Users* Facebook::SearchUser(char*& FriendID)
{
	Users* UserID = 0;

	for (int i = 0; i < TotalUsers; i++)
	{
		UserID = UserList[i]->SearchUserByID(FriendID);

		if (UserID)
			return UserID; // returning the Address or '0'
	}

	return 0;
}

Object* Facebook::SearchObject(char object[])
{
	if (object[0] == 'u')
	{
		Users* user = SearchUser(object);
		return user;
	}


	else if (object[0] == 'p')
	{
		Pages* page = SearchPage(object);
		return page;
	}

	return 0;
}

Posts* Facebook::SearchPost(char*& tempID)
{
	Posts* PostID = 0;

	for (int i = 0; i < TotalPosts; i++)
	{
		PostID = PostsList[i]->SearchPostByID(tempID);

		if (PostID)
			return PostID;
	}

	return 0;
}

Users* Facebook::SetCurrentUser(const char User[])
{
	cout << "Command:\tSet current user \"" << User << "\"\n";

	char* userID = Helper::GetStringFromConstLiteral(User);

	Users* CurrentUser = 0;
	CurrentUser = SearchUser(userID);

	if (userID)
		delete[] userID;

	if (CurrentUser)
	{
		cout << "\n";  CurrentUser->PrintFullName(); cout << "successfully set as current user\n";
	}

	else
		cout << "\nUser not found\n\n";

	return CurrentUser;
}

void Facebook::ViewPostLikes(const char Post[])
{
	cout << "\nCommand:\tView Post's Likes\n\n";

	char* postID = Helper::GetStringFromConstLiteral(Post);

	if (postID)
	{
		Posts* post = SearchPost(postID);

		if (post)
			post->PrintLikes();

		delete[] postID;
	}

}

void Facebook::ViewPost(const char Post[])
{
	cout << "\nCommand:\tView Post\n\n";
	cout << "------------------------------------------------------\n\n";

	char* postID = Helper::GetStringFromConstLiteral(Post);

	if (postID)
	{
		Posts* post = SearchPost(postID);

		if (post)
			post->PrintPost();

		else
			cout << "Post Not Found\n";

		delete[] postID;
	}

	cout << "------------------------------------------------------\n\n";
}

void Facebook::ViewPage(const char Page[])
{
	cout << "Command:\tView Page\n\n";

	char* pageID = Helper::GetStringFromConstLiteral(Page);

	if (pageID)
	{
		Pages* page = SearchPage(pageID);

		if (page)
			page->PrintAllPosts();

		else
			cout << "Page Not Found\n";

		delete[] pageID;
	}
}

void Facebook::LikePost(const char Post[])
{
	cout << "\nCommand:\tLike Post\n\n";

	char* postID = Helper::GetStringFromConstLiteral(Post);

	if (postID)
	{
		Posts* post = SearchPost(postID);

		if (post)
			post->LikePost(CurrentUser);

		delete[] postID;
	}
}

void Facebook::PostComment(const char Post[], const char text[])
{
	cout << "Command:\tPost Comment\n\n";

	char* postID = Helper::GetStringFromConstLiteral(Post);
	char* caption = Helper::GetStringFromConstLiteral(text); // caption will be deleted in Comment's Destructor

	if (postID)
	{
		Posts* post = SearchPost(postID);

		if (post && caption)
		{
			post->AddNewComment(caption, CurrentUser, TotalComments);
			SetTotalComments(TotalComments + 1);
		}

		delete[] postID;
	}
}

void Facebook::ShareMemories(const char Post[], const char text[])
{
	cout << "Command:\tShare Memory\n\n";

	char* postID = Helper::GetStringFromConstLiteral(Post);
	char* caption = Helper::GetStringFromConstLiteral(text); // this caption is deleted in Post's Destructor through Memory***

	if (postID)
	{
		Posts* post = Facebook::SearchPost(postID);

		if (post)
			CurrentUser->ShareMemories(post, caption, TotalPosts);

		delete[] postID;
	}
}

// Object Class

Object::Object()
{
	ObjectID = 0;
}

Object::~Object()
{
	cout << "~Object() Called\n";

	if (ObjectID)
		delete[] ObjectID;

}

void Object::SetID(char*& ID)
{
	ObjectID = ID;
}

void Object::PrintID()
{
	if (ObjectID)
		cout << ObjectID;
}

char* Object::GetObjectID()
{
	return ObjectID;
}

void Object::AddCommentor(Object*& object)
{
	AddCommentor(object);
}

// Users Class

Users::Users()
{
	FirstName = 0;
	LastName = 0;
	Friends = 0;
	LikedPages = 0;
	Timeline = 0;
	num_friends = 0;
	num_LikedPages = 0;
	num_Posts = 0;
}

Users::~Users()
{
	//cout << "~Users() Called\n";

	if (FirstName)
		delete[] FirstName;

	if (LastName)
		delete[] LastName;

	if (Friends)
		delete[] Friends;

	if (LikedPages)
		delete[] LikedPages;

	if (Timeline)
	{
		for (int i = 0; i < num_Posts; i++)
		{
			if (Timeline[i])
			{
				delete Timeline[i]; Timeline[i] = 0;
			}
		}

		delete[] Timeline;
	}
}

void Users::ReadUsers(ifstream& UsersFile)
{
	char temp[20], * tempID = 0;
	UsersFile >> temp;
	tempID = Helper::GetStringFromBuffer(temp);
	SetID(tempID);

	UsersFile >> temp;
	FirstName = Helper::GetStringFromBuffer(temp);

	UsersFile >> temp;
	LastName = Helper::GetStringFromBuffer(temp);
}

void Users::AssociatePages(Pages*& PagePtr)
{
	if (PagePtr && num_LikedPages == 0)
	{
		LikedPages = new Pages * [10];
	}

	if (PagePtr)
		LikePage(PagePtr);
}

void Users::LikePage(Pages*& PagePtr)
{
	if (num_LikedPages < 10)
		LikedPages[num_LikedPages] = PagePtr;

	if (num_LikedPages < 9)
		LikedPages[num_LikedPages + 1] = 0;

	num_LikedPages++;
}

void Users::AssociateFriends(Users*& FriendPtr)
{
	if (FriendPtr && num_friends == 0)
	{
		Friends = new Users * [10];
	}

	if (FriendPtr)
		AddFriend(FriendPtr);
}

Users* Users::SearchUserByID(char*& FriendID)
{
	char* ObjectID = GetObjectID();
	int length1 = Helper::StringLength(ObjectID);
	int length2 = Helper::StringLength(FriendID);

	if (length1 != length2)
		return 0;

	bool flag = true;

	for (int i = 0; i < length1; i++)
	{
		if (FriendID[i] != ObjectID[i])
		{
			flag = false;
		}
	}

	if (flag)
		return this;

	else
		return 0;
}

void Users::AddFriend(Users*& FriendPtr) // Continuation of AssociateFriends()
{
	if (num_friends < 10)
	{
		Friends[num_friends] = FriendPtr;
		num_friends++;
	}

	if (num_friends < 9)
		Friends[num_friends] = 0;
}

void Users::AddPostToTimeline(Posts*& post)
{
	if (post && num_Posts == 0)
	{
		Timeline = new Posts * [10];
	}

	if (num_Posts < 10)
		Timeline[num_Posts] = post;

	if (num_Posts < 9)
		Timeline[num_Posts + 1] = 0;

	num_Posts++;
}

void Users::AccessTimeline()
{
	for (int i = 0; Timeline && Timeline[i] != 0; i++)
	{
		if (Timeline[i])
			Timeline[i]->PrintPost();
	}
}

void Users::PrintFriends()
{
	bool flag = 0;

	cout << "------------------------------------------------------\n\n";

	cout << FirstName << " " << LastName << " - Friend List\n\n";

	for (int i = 0; Friends && i < num_friends; i++)
	{
		flag = true;

		Friends[i]->PrintID();
		cout << " - ";

		cout << Friends[i]->FirstName << " " << Friends[i]->LastName << endl;
	}

	if (!flag)
		cout << "No Friends found.\n";

	cout << "\n------------------------------------------------------\n";
}

void Users::PrintPages()
{
	bool flag = 0;

	cout << "------------------------------------------------------\n\n";

	cout << FirstName << " " << LastName << " - Liked Pages\n\n";

	for (int i = 0; LikedPages && i < num_LikedPages; i++)
	{
		flag = true;

		LikedPages[i]->PrintPages();
	}

	if (!flag)
		cout << "No Liked Pages found.\n";

	cout << "\n------------------------------------------------------\n";
}

void Users::PrintLatestPosts()
{
	bool flag = false;

	cout << "------------------------------------------------------\n\n";

	// Print Friend's Posts

	cout << "--- " << FirstName << " " << LastName << " --- Home\n\n";

	for (int i = 0; Friends && Friends[i]; i++)
	{
		flag = true;

		Friends[i]->AccessTimeline();
	}

	// Print Liked Posts

	for (int i = 0; LikedPages && LikedPages[i]; i++)
	{
		flag = true;

		LikedPages[i]->AccessTimeline();
	}

	if (!flag)
		cout << "No Latest Posts\n";

	cout << "------------------------------------------------------\n";
}

void Users::PrintFullName()
{
	cout << FirstName << " " << LastName << " ";
}

//-------------------------------\\

void Users::PrintListView()
{
	PrintID();
	cout << " -- " << FirstName << " " << LastName << endl;
}

void Users::ViewFriendList()
{
	cout << "\nCommand:\tView Friend List\n\n";

	PrintFriends();
}

void Users::ViewLikedPages()
{
	cout << "\nCommand:\tView Liked Pages\n\n";

	PrintPages();
}

void Users::ViewHome()
{
	cout << "\nCommand:\tView Home\n\n";

	PrintLatestPosts();
}

void Users::SeeMemories()
{
	bool flag = false;

	cout << "Command:\tSee Memories\n\n";
	cout << "------------------------------------------------------\n\n";

	cout << "We hope you enjoy looking back and sharing your memories on Facebook, from the most recent to those long ago.\n\n";

	for (int i = 0; Timeline && Timeline[i]; i++)
	{
		flag = true;
		Timeline[i]->ShowMemories(); // checks each post and if date matches prints it (for memory)
	}

	if (!flag)
		cout << "No Memories found\n\n";

	cout << "------------------------------------------------------\n\n";
}

void Users::ShareMemories(Posts*& post, char*& caption, int TotalPosts)
{
	bool flag = false;

	for (int i = 0; Timeline && i < num_Posts; i++)
	{
		if (Timeline[i] == post)
			flag = true;
	}

	if (flag)
	{
		if (num_Posts > 0 && num_Posts < 10)
		{
			Timeline[num_Posts] = new Memory(this, caption, post, TotalPosts);
			num_Posts++;
		}

		if (num_Posts < 9)
			Timeline[num_Posts] = 0;
	}

	if (!flag)
		cout << "No Memory found to share\n\n";
}

void Users::ViewTimeline()
{
	bool flag = false;

	cout << "Command:\tView Timeline\n\n";
	cout << "------------------------------------------------------\n\n";

	PrintFullName(); cout << "-- Timeline\n";

	if (Timeline && Timeline[num_Posts - 1])
	{
		flag = true;
		Timeline[num_Posts - 1]->PrintPost(true);
	}

	for (int i = 0; Timeline && i < num_Posts - 1; i++)
	{
		flag = true;
		Timeline[i]->PrintPost(true);
	}

	if (!flag)
		cout << "\nNo Latest Posts\n\n";

	cout << "------------------------------------------------------\n\n";
}

// Pages Class

Pages::Pages()
{
	PageName = 0;
	Timeline = 0;
	num_posts = 0;
	num_LikedPosts = 0;
}

Pages::~Pages()
{
	cout << "~Pages() Called\n";

	if (PageName)
		delete[] PageName;

	if (Timeline)
	{
		for (int i = 0; i < num_posts; i++)
		{
			if (Timeline[i])
			{
				delete Timeline[i]; Timeline[i] = 0;
			}
		}

		delete[] Timeline;
	}
}

void Pages::ReadPages(ifstream& PagesFile)
{
	char temp[50], * tempID = 0;
	PagesFile >> temp;
	tempID = Helper::GetStringFromBuffer(temp);
	SetID(tempID);

	PagesFile.ignore();
	PagesFile.getline(temp, 49);
	PageName = Helper::GetStringFromBuffer(temp);
}

Pages* Pages::SearchPageByID(char*& tempID)
{
	char* ObjectID = GetObjectID();
	int length1 = Helper::StringLength(ObjectID);
	int length2 = Helper::StringLength(tempID);

	if (length1 != length2)
		return 0;

	bool flag = true;

	for (int i = 0; i < length1; i++)
	{
		if (ObjectID[i] != tempID[i])
		{
			flag = false;
		}
	}

	if (flag)
		return this;

	else
		return 0;
}

void Pages::PrintPages()
{
	PrintID();
	cout << " - " << PageName << endl;
}

void Pages::AddPostToTimeline(Posts*& post)
{
	if (post && num_posts == 0)
	{
		Timeline = new Posts * [10];
	}

	if (num_posts < 10)
	{
		Timeline[num_posts] = post;
		num_posts++;
	}

	if (num_posts < 9)
		Timeline[num_posts] = 0;
}

void Pages::AccessTimeline()
{
	for (int i = 0; Timeline && Timeline[i] != 0; i++)
	{
		if (Timeline[i])
			Timeline[i]->PrintPost();
	}
}

void Pages::PrintFullName()
{
	cout << PageName << " ";
}

void Pages::PrintListView()
{
	PrintID();
	cout << " -- " << PageName << endl;
}

void Pages::PrintAllPosts()
{
	cout << "------------------------------------------------------\n\n";

	cout << PageName << "\n\n";

	for (int i = 0; Timeline && Timeline[i]; i++)
	{
		Timeline[i]->PrintPostDetails(true);
	}

	cout << "------------------------------------------------------\n\n";
}

// Date Class

Date::Date()
{
	Day = 0;
	Month = 0;
	Year = 0;
}

void Date::ReadDate(ifstream& PostsFile)
{
	PostsFile >> Day;
	PostsFile >> Month;
	PostsFile >> Year;
}

void Date::SetDate(int day, int month, int year)
{
	Day = day;
	Month = month;
	Year = year;
}

void Date::SetSystemDate(int day, int month, int year)
{
	cout << "Command:\tSet System Date\n\n";

	SystemDate.SetDate(day, month, year);

	cout << "System Date:\t";
	SystemDate.PrintDate();
	cout << endl << endl;
}

bool Date::CheckDate(int day, int month, int year)
{
	if (Year == year && Month == month)
	{
		if (Day == day || Day == day - 1)
		{
			return true;
		}
	}

	return false;
}

void Date::PrintDate()
{
	cout << "(" << Day << "/" << Month << "/" << Year << ")";
}

int Date::CompareDateForMemory(Date& RHS)
{
	if ((Day == RHS.Day) && (Month == RHS.Month))
		return (RHS.Year - Year);

	else
		return 0;
}

// Posts Class

Posts::Posts()
{
	PostType = 0;
	PostID = 0;
	activity = 0;
	Text = 0;
	SharedBy = 0;
	LikedBy = 0;
	comments = 0;
	no_Likes = 0;
	no_Comments = 0;
}

Posts::Posts(Users*& user, char*& caption)
{
	PostType = 1;
	PostID = 0;
	date = Date::SystemDate;
	activity = 0;
	Text = caption;
	SharedBy = user;
	LikedBy = 0;
	comments = 0;

	no_Comments = 0;
	no_Likes = 0;
}

void Posts::SetPostID(int TotalPosts)
{
	// Storing New PostID

	char* tempID = Helper::IntToChar(TotalPosts + 1);

	char temp[] = { 'p', 'o', 's', 't', tempID[0], tempID[1], '\0' };

	PostID = Helper::GetStringFromBuffer(temp);

	if (tempID)
		delete[] tempID;
}

Posts::~Posts()
{
	cout << "~Posts() Called\n";

	if (PostID)
		delete[] PostID;

	if (activity)
		delete activity;

	if (Text)
		delete[] Text;

	if (comments)
	{
		for (int i = 0; i < no_Comments; i++)
		{
			if (comments[i])
			{
				delete comments[i]; comments[i] = 0;
			}
		}

		delete[] comments;
	}
}

void Posts::ReadPosts(ifstream& PostsFile)
{
	char temp[100];

	PostsFile >> PostType;

	PostsFile >> temp;
	PostID = Helper::GetStringFromBuffer(temp);

	date.ReadDate(PostsFile);

	PostsFile.ignore();
	PostsFile.getline(temp, 99);
	Text = Helper::GetStringFromBuffer(temp);

	if (PostType == 2)
	{
		activity = new Activity;

		activity->ReadData(PostsFile);
	}
}

Posts* Posts::SearchPostByID(char*& tempID)
{
	int length1 = Helper::StringLength(PostID);
	int length2 = Helper::StringLength(tempID);

	if (length1 != length2)
		return 0;

	bool flag = true;

	for (int i = 0; i < length1; i++)
	{
		if (tempID[i] != PostID[i])
		{
			flag = false;
		}
	}

	if (flag)
		return this;

	else
		return 0;
}

void Posts::SetSharedBy(Object*& sharer)
{
	SharedBy = sharer;
}

void Posts::AddLikeToPost(Object*& LikedID)
{
	if (LikedID && no_Likes == 0)
	{
		LikedBy = new Object * [10];
	}

	if (LikedID)
		LikePost(LikedID);
}

void Posts::LikePost(Object* user)
{
	if (no_Likes < 10)
	{
		LikedBy[no_Likes] = user;
		no_Likes++;
	}

	if (no_Likes < 9)
		LikedBy[no_Likes] = 0;
}

void Posts::AddComment(Comments*& comment)
{
	if (no_Comments == 0)
	{
		comments = new Comments * [10];
	}

	if (no_Comments < 10)
	{
		comments[no_Comments] = comment;
		no_Comments++;
	}

	if (no_Comments < 9)
		comments[no_Comments] = 0;
}

void Posts::AddNewComment(char*& text, Users*& user, int TotalComments)
{
	if (no_Comments == 0)
	{
		comments = new Comments * [10];
	}

	if (no_Comments > 0 && no_Comments < 10)
	{
		comments[no_Comments] = new Comments(text, user, TotalComments);
		no_Comments++;

		if (no_Comments != 10)
			comments[no_Comments] = 0;
	}
}

void Posts::PrintPost(bool PrintDate)
{
	if (!PrintDate)
	{
		if (date.CheckDate(15, 11, 2017))
			PrintPostDetails();
	}

	else
		PrintPostDetails(PrintDate);
}

void Posts::PrintLikes()
{
	cout << "------------------------------------------------------\n";
	cout << "\nPost Liked By:\n\n";

	for (int i = 0; LikedBy[i] != 0; i++)
	{
		LikedBy[i]->PrintListView();
	}

	cout << "\n------------------------------------------------------\n";
}

void Posts::PrintPostDetails(bool PrintDate, bool PrintComments)
{
	if (!PrintComments)
	{
		int difference = date.CompareDateForMemory(Date::SystemDate);
		cout << "\t\t\t(" << difference << " Years Ago)\n";
	}

	cout << "-- ";
	SharedBy->PrintFullName();

	if (PostType == 2)
	{
		activity->PrintActivity();
	}

	else
		cout << "shared ";

	cout << "\"" << Text;

	if (PrintDate)
	{
		cout << "\" .....";
		date.PrintDate();
	}

	if (PrintComments)
	{
		for (int i = 0; i < no_Comments; i++)
		{
			comments[i]->PrintComments();
		}
	}

	cout << "\n\n";
}

void Posts::ShowMemories()
{
	int difference = date.CompareDateForMemory(Date::SystemDate);

	if (difference)		// not a memory if diff <=0
	{
		cout << "On this Day\n";
		cout << difference << " Years Ago\n";
		PrintPostDetails();
	}
}

// Memory Class

Memory::Memory()
{
	OriginalPost = 0;
}

Memory::Memory(Users* user, char*& caption, Posts*& OG_Post, int TotalPosts) : Posts(user, caption)
{
	OriginalPost = OG_Post;
	SetPostID(TotalPosts);
}

Memory::~Memory()
{
	cout << "~Memory() Called\n";

	OriginalPost = 0;
}

void Memory::PrintPost(bool PrintDate)
{
	cout << "\n`````";
	SharedBy->PrintFullName();

	cout << "shared a Memory`````....";
	date.PrintDate();

	cout << "\n\"" << Text << "\"\n";

	OriginalPost->PrintPostDetails(true, false);

	for (int i = 0; i < no_Comments; i++) // these are the comments of the Memory Post and will be printed instead						
	{									  // of the comments of the original post
		comments[i]->PrintComments();
	}
}

// Activity Class

Activity::Activity()
{
	Type = 0;
	Value = 0;
}

Activity::~Activity()
{
	cout << "~Activity() Called\n";

	if (Value)
		delete[] Value;
}

void Activity::ReadData(ifstream& PostsFile)
{
	PostsFile >> Type;

	char temp[50];
	PostsFile.ignore();
	PostsFile.getline(temp, 49);
	Value = Helper::GetStringFromBuffer(temp);
}

void Activity::PrintActivity()
{
	if (Type == 1)
		cout << "is feeling ";

	else if (Type == 2)
		cout << "is thinking about ";

	else if (Type == 3)
		cout << "is making ";

	else if (Type == 4)
		cout << "is celebrating ";

	cout << Value << "\n   ";
}

// Comments Class

Comments::Comments()
{
	CommentID = 0;
	Text = 0;
	Commentor = 0;
}

Comments::Comments(char*& text, Object* user, int TotalComments)
{
	Text = text;		// this is the infamous caption from PostComment()
	Commentor = user;
	SetCommentID(TotalComments);
}

void Comments::SetCommentID(int TotalComments)
{
	char* tempID = Helper::IntToChar(TotalComments + 1);

	char temp[] = { 'c', tempID[0], tempID[1], '\0' };

	CommentID = Helper::GetStringFromBuffer(temp);

	if (tempID)
		delete[] tempID;
}

Comments::~Comments()
{
	cout << "~Comments() Called\n";

	if (CommentID)
		delete[] CommentID;

	if (Text)
		delete[] Text;
}

void Comments::ReadComments(ifstream& CommentsFile)
{
	char temp[100];

	CommentsFile >> temp;
	CommentID = Helper::GetStringFromBuffer(temp);
}

void Comments::ReadText(ifstream& CommentsFile)
{
	char temp[100];
	CommentsFile.ignore();
	CommentsFile.getline(temp, 99);
	Text = Helper::GetStringFromBuffer(temp);
}

void Comments::AddCommentor(Object*& object)
{
	if (object)
		Commentor = object;
}

void Comments::PrintComments()
{
	cout << "\n\t\t";
	Commentor->PrintFullName();

	cout << "wrote: \"" << Text << "\"";
}

// Helper Class

char* Helper::GetStringFromBuffer(char* buffer)
{
	int length = StringLength(buffer);

	if (length)
	{
		char* string = new char[length + 1];

		StringCopy(buffer, string);

		return string;
	}

	else
		return 0;
}

char* Helper::GetStringFromConstLiteral(const char buffer[])
{
	int length = StringLength(buffer);

	if (length)
	{
		char* string = new char[length + 1];

		StringCopy(buffer, string);

		return string;
	}

	else
		return 0;
}

char* Helper::StringCopy(const char* source, char* dest)
{
	int i = 0;

	for (i = 0; source[i] != '\0'; i++)
	{
		dest[i] = source[i];
	}

	dest[i] = '\0';

	return dest;
}

int Helper::StringLength(const char* string)
{
	int length = 0;

	for (int i = 0; string[i] != '\0'; i++)
	{
		length++;
	}

	return length;
}

char* Helper::IntToChar(int num)
{
	char* string;

	if (num < 10)
		string = (char*)(num + 48);

	else
	{
		string = new char[3];

		string[0] = num / 10 + '0';
		string[1] = num % 10 + '0';
		string[2] = '\0';
	}

	return string;
}