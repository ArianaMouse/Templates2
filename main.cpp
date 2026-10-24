#include <iostream>
#include <string>
#include <cstddef> //size_t.

class VeryHeavyDatabase {
public:
    virtual std::string GetData(const std::string& key) const noexcept {
        return "value";
    }
    virtual ~VeryHeavyDatabase() = default;
};

class OneShotDB : public VeryHeavyDatabase {
private:
    VeryHeavyDatabase* real_object_;
    mutable size_t shots_; //mutable, так как изменяем в const-методе.

public:
    explicit OneShotDB(VeryHeavyDatabase* real_object, size_t shots = 1)
        : real_object_(real_object), shots_(shots) {
    }

    std::string GetData(const std::string& key) const noexcept override {
        if (shots_ > 0) {
            --shots_;
            return real_object_->GetData(key);
        }
        return "error";
    }
};

int main() {
    auto real_db = VeryHeavyDatabase();
    auto limit_db = OneShotDB(std::addressof(real_db), 2);

    std::cout << limit_db.GetData("key") << std::endl; //value.
    std::cout << limit_db.GetData("key") << std::endl; //value.
    std::cout << limit_db.GetData("key") << std::endl; //error.

    return 0;
}