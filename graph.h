#include <string>
#include <vector>
class node;
class graph
{
    std::vector< std::shared_ptr<node> > nodes;
public:
    void addWordPair(std::string first, std::string second);
    void addStartWord(std::string word);
    void addEndWord(std::string word);
    std::string getSentence() const;
};
