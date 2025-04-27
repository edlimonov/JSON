#pragma once

#include "json.h"
#include <optional>

namespace json {

class BaseContext;
class DictItemContext;
class ArrayItemContext;
class KeyItemContext;

class Builder {
public:
    
    Builder() = default;
    DictItemContext StartDict();
    ArrayItemContext StartArray();
    Builder& EndDict();
    Builder& EndArray();
    KeyItemContext Key(std::string key);
    Builder& Value(Node::Value value);
    Node Build();
    
private:
    std::optional<Node> root_;
    std::vector<Node*> nodes_stack_;
};

class BaseContext {
public:
    BaseContext(Builder& builder): builder_(builder){}
    DictItemContext StartDict();
    ArrayItemContext StartArray();
    Builder& EndDict();
    Builder& EndArray();
    KeyItemContext Key(std::string key);
    Builder& Value(Node::Value value);
    
private:
    Builder& builder_;
};

class DictItemContext: public BaseContext {
public:
    DictItemContext(Builder& builder);
    
    ArrayItemContext StartArray() = delete;
    DictItemContext StartDict() = delete;
    Builder& EndArray() = delete;
    Builder& Value(Node::Value value) = delete;
};

class ArrayItemContext: public BaseContext {
public:
    ArrayItemContext(Builder& builder);
    
    Builder& EndDict() = delete;
    KeyItemContext Key(std::string key) = delete;
};

class KeyItemContext: public BaseContext {
public:
    KeyItemContext(Builder& builder);
    
    Builder& EndDict() = delete;
    Builder& EndArray() = delete;
    KeyItemContext Key(std::string key) = delete;
};

}
