#include <string>
#include <vector>
#include <memory>
class node;
class graph
{
    std::vector< node* > nodes;
public:
    void addWordPair(std::string first, std::string second);
    void addStartWord(std::string word);
    void addEndWord(std::string word);
    std::string getSentence() const;
};
