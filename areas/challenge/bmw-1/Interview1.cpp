/* In this exercise we want to design a Storyboard. Our version of the Storyboard

 * contains arbitrary many notes (imagine it like putting sticky notes on a board).

 * Every note has a title, a text and a set of tags. E.g.

 * 	- title: "Test Traceplayer"

 * 	- text: "Implement a unit test for the class Traceplayer of the spark core framework."

 * 	- tags: {"unit test", "traceplayer", "testing", "spark core"}

 * 

 * Our Storyboard should enable us to search for notes by title, text and tags.

 * E.g.:

 *      searchByTitle("Test Traceplayer")

 *      searchByTag({"testing", "unit test"})

 *   	searchByText("Implement a unit test for the class Traceplayer of the spark core framework.")

 * For the sake of simplicity we don't want to do any similiarity or prefix matching when

 * searching for a title, tag or text. Only an exact match should give results.

 * 

 * The skeleton code below can be used as a starting point but doesn't have to.

 * The comments "fill in" are placeholders where you definitely have to put in some code when 

 * you use this skeleton code. But this doesn't mean that you shouldn't or can't put code anywhere else.

 * 

 * Also write some simple unit tests to show how you would test the functionality of the Storyboard.

 * Don't use any testing framework. Simple if-statements are sufficient for this exercise.

 * 

 * Hint: Think about performance versus memory tradeoffs in your design, so you can give good 

 *       reasons for your decision. 

 */

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Note
{
public:

	using IdT = std::size_t;
	using TitleT = std::string;
	using TextT = std::string;
	using TagT = std::string;
	using TagListT = std::unordered_set<TagT>;

	Note(TitleT title, TextT text)
	: id_(generateId()),
	title_(std::move(title)),
	text_(text)
	{
	}

	TitleT title() const
	{
		return title_;
	}

	TextT text() const
	{
		return text_;
	}

	TagListT tags() const
	{
		return tags_;
	}

	void addTag(std::initializer_list<TagT> tags)
	{
		for (auto tag : tags)
		{
			tags_.emplace(std::move(tag));
		}
	}

	void removeTag(const TagT& tag)
	{
		tags_.erase(tag);
	}

	IdT id() const
	{
		return id_;
	}

private:
	friend std::hash<Note>;

	static IdT generateId() 
	{
		static IdT lastId = 0;
		return ++lastId;
	}

	const IdT id_;
	TitleT title_;
	TextT text_;
	TagListT tags_;
};

class Storyboard
{
public:
	using NoteCPtr = std::shared_ptr<const Note>;
	using NoteCList = std::vector<NoteCPtr>;

	void addNote(Note note)
	{
		const auto id = note.id();
		titleIndex_.emplace(std::make_pair(note.title(), id));
		textIndex_.emplace(std::make_pair(note.text(), id));
		for (auto tag : note.tags())
		{
			tagIndex_.emplace(std::make_pair(std::move(tag), id));
		}
		notes_.emplace(
			std::make_pair(id, std::make_shared<Note>(std::move(note))));
	}

	void deleteNote(Note const& note)
	{
		notes_.erase(note.id());
		removeFromIndex(titleIndex_, note.title(), note.id());
		removeFromIndex(textIndex_, note.text(), note.id());
		for (auto tag : note.tags())
		{
			removeFromIndex(tagIndex_, tag, note.id());
		}
	}

	NoteCList searchByTitle(const Note::TitleT& title) const
	{
		auto result = NoteCList();
		searchInIndex(result, titleIndex_, title);
		return result;
	}

	NoteCList searchByText(const Note::TextT& text) const
	{
		auto result = NoteCList();
		searchInIndex(result, textIndex_, text);
		return result;
	}

	NoteCList searchByTag(std::initializer_list<Note::TagT> tags) const
	{
		auto result = NoteCList();
		for (auto tag : tags)
		{
			searchInIndex(result, tagIndex_, tag);
		}
		return result;
	}

private:
	using NotePtr = std::shared_ptr<Note>;

	template <typename Container, typename Key>
	void removeFromIndex(Container& cont, const Key& key, Note::IdT id)
	{
		auto matching = cont.equal_range(key);
		std::vector<decltype(matching.first)> entriesToRemove;
		entriesToRemove.reserve(std::distance(matching.first, matching.second));
		for (auto match = matching.first; match != matching.second; ++match)
		{
			if (match->second == id)
			{
				entriesToRemove.push_back(match);
			}
		}
		for (auto toRemove : entriesToRemove)
		{
			cont.erase(toRemove);
		}
	}

	template <typename Container, typename Key>
	void searchInIndex(
		NoteCList& result, const Container& cont, const Key& key) const
	{
		auto matching = cont.equal_range(key);
		auto matchingCount = std::distance(matching.first, matching.second);
		result.reserve(matchingCount + result.size());
		for (auto match = matching.first; match != matching.second; ++match)
		{
			auto foundNote = notes_.find(match->second);
			if (foundNote != notes_.cend())
			{
				result.push_back(foundNote->second);
			}
		}
	}

	std::unordered_map<Note::IdT, NotePtr> notes_;
	std::unordered_multimap<Note::TitleT, Note::IdT> titleIndex_;
	std::unordered_multimap<Note::TextT, Note::IdT> textIndex_;
	std::unordered_multimap<Note::TagT, Note::IdT> tagIndex_;
};

 std::ostream& operator<<(
	std::ostream& str, const std::unordered_set<std::string>& tags)
{
	auto tagVec = std::vector<std::string>(tags.cbegin(), tags.cend());
	str << "[ ";
	const auto sizeMinusOne = tagVec.size() - 1;
	for (auto i = 0; i < sizeMinusOne; ++i)
	{
		str << tagVec[i] << ", ";
	}
	if (!tagVec.empty())
	{
		str << tagVec.back();
	}
	str << " ]";
	return str;
}

template <typename T>
bool ReportIfNotEqual(const T& lhs, const T& rhs, 
	const char* lhsText, const char* rhsText)
{
	auto equal = lhs == rhs;
	if (!equal)
	{
		std::cerr << "Info: not equal => " << lhsText << " != " << rhsText <<
			"!" << std::endl << 
			"    " << lhsText << " = " << lhs << std::endl << 
			"    " << rhsText << " = " << rhs << std::endl;
			;
	}
	return equal;
}

#define TEST_EQ(LHS, RHS) ReportIfNotEqual(LHS, RHS, #LHS, #RHS)

bool operator==(const Note& note1, const Note& note2)
{
	return TEST_EQ(note1.id(), note2.id()) &&
		TEST_EQ(note1.title(), note2.title()) &&
		TEST_EQ(note1.text(), note2.text()) &&
		TEST_EQ(note1.tags(), note2.tags());
}

void Test(int lineNum, const char* context, bool pass)
{
	if (!pass)
	{
		std::cerr << "Test failed: on line number " << lineNum << ", " << 
			context << " is supposed to be true but is not!" << std::endl;
	}
}

#define TEST(PASS) Test(__LINE__, #PASS, PASS)

int main(int argc, char** argv)
{
	{
		std::unordered_map<Note::IdT, Note> uniqueIds;
		const auto numberOfNotes = 1000;
		for (auto i = 0; i < numberOfNotes; ++i)
		{
			uniqueIds.emplace(std::make_pair(i, Note("title", "text")));
		}
		TEST(uniqueIds.size() == numberOfNotes);

		auto storyboard = Storyboard();
		TEST(storyboard.searchByTitle("").empty());
		TEST(storyboard.searchByTitle("Test Traceplayer").empty());
		TEST(storyboard.searchByText("").empty());
		TEST(storyboard.searchByText("Implement a unit test for the class Traceplayer of the spark core framework.").empty());
		TEST(storyboard.searchByTag({}).empty());
		TEST(storyboard.searchByTag({"unit test"}).empty());
		TEST(storyboard.searchByTag({"traceplayer"}).empty());
		TEST(storyboard.searchByTag({"testing", "spark core"}).empty());

		auto note = Note("Test Traceplayer", 
			"Implement a unit test for the class Traceplayer of the spark core framework.");
		note.addTag({"unit test", "traceplayer", "testing", "spark core"});
		
		storyboard.addNote(note);

		TEST(storyboard.searchByTitle("").empty());
		TEST(storyboard.searchByTitle("non-existing").empty());

		auto searchByTitle = storyboard.searchByTitle("Test Traceplayer");
		TEST(searchByTitle.size() == 1);
		TEST(*searchByTitle.front() == note);

		TEST(storyboard.searchByText("").empty());
		TEST(storyboard.searchByText("non-existing").empty());
		TEST(storyboard.searchByText(
			"Implement a unit test for the class Traceplayer").empty());

		auto searchByText = storyboard.searchByText(
			"Implement a unit test for the class Traceplayer of the spark core framework.");
		TEST(searchByText.size() == 1);
		TEST(*searchByText.front() == note);

		TEST(storyboard.searchByTag({}).empty());
		TEST(storyboard.searchByTag({""}).empty());

		auto searchByTagSparkCore = storyboard.searchByTag({"spark core"});
		TEST(searchByTagSparkCore.size() == 1);
		TEST(*searchByTagSparkCore.front() == note);

		auto searchByTagSparkCoreAndSomethingAndEmpty = 
			storyboard.searchByTag({"spark core", "something", ""});
		TEST(searchByTagSparkCoreAndSomethingAndEmpty.size() == 1);
		TEST(*searchByTagSparkCoreAndSomethingAndEmpty.front() == note);

		auto dupeDataNote = Note(note.title(), note.text());
		dupeDataNote.addTag({"unit test", "traceplayer", "testing", "spark core"});

		storyboard.addNote(dupeDataNote);
		auto searchByOldTitle = storyboard.searchByTitle(note.title());
		TEST(searchByOldTitle.size() == 2);
		TEST(*searchByOldTitle.front() == note || 
			*searchByOldTitle.back() == note);
		TEST(*searchByOldTitle.front() == dupeDataNote || 
			*searchByOldTitle.back() == dupeDataNote);

		storyboard.deleteNote(dupeDataNote);
		searchByTitle = storyboard.searchByTitle("Test Traceplayer");
		TEST(searchByTitle.size() == 1);
		TEST(*searchByTitle.front() == note);

		auto anotherNote = Note("Test Tracerecorder", 
			"Implement a unit test for the class Tracerecorder of the spark core framework.");
		anotherNote.addTag({"unit test", "tracerecorder", "testing", "spark core"});

		storyboard.deleteNote(anotherNote); // Nothing happens
		storyboard.addNote(anotherNote);

		searchByTitle = storyboard.searchByTitle("Test Tracerecorder");
		TEST(searchByTitle.size() == 1);
		TEST(*searchByTitle.front() == anotherNote);
		searchByTitle = storyboard.searchByTitle("Test Traceplayer");
		TEST(searchByTitle.size() == 1);
		TEST(*searchByTitle.front() == note);
	}
	return 0;
}