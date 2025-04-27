#include "json_builder.h"

namespace json {
DictItemContext Builder::StartDict(){
    
    if (!root_.has_value()){
        root_ = Node(Dict());
        nodes_stack_.push_back(&root_.value());
        return DictItemContext{*this};
    }
    
    //объект уже создан
    if (nodes_stack_.empty()){
        throw std::logic_error("");
    }
    
    if (nodes_stack_.back()->IsNull()){
        *nodes_stack_.back() = Node(Dict());
        return DictItemContext{*this};
    }
    
    if (nodes_stack_.back()->IsArray()){
        std::get<Array>(nodes_stack_.back()->GetValue()).push_back(Node(Dict()));
        nodes_stack_.push_back(&std::get<Array>(nodes_stack_.back()->GetValue()).back());
        return DictItemContext{*this};
    }
    
    throw std::logic_error("");
}

ArrayItemContext Builder::StartArray(){
    
    if (!root_.has_value()){
        root_ = Node(Array());
        nodes_stack_.push_back(&root_.value());
        return ArrayItemContext{*this};
    }
    
    //объект уже создан
    if (nodes_stack_.empty()){
        throw std::logic_error("");
    }
    
    if (nodes_stack_.back()->IsNull()){
        *nodes_stack_.back() = Node(Array());
         return ArrayItemContext{*this};
    }
    
    if (nodes_stack_.back()->IsArray()){
        std::get<Array>(nodes_stack_.back()->GetValue()).push_back(Node(Array()));
        nodes_stack_.push_back(&std::get<Array>(nodes_stack_.back()->GetValue()).back());
        return ArrayItemContext{*this};
    }
    
    throw std::logic_error("");
}

Builder& Builder::EndDict(){
    if (!nodes_stack_.empty() && nodes_stack_.back()->IsDict()){
        nodes_stack_.pop_back();
        return *this;
    }
    
    throw std::logic_error("");
}

Builder& Builder::EndArray(){
    if (!nodes_stack_.empty() && nodes_stack_.back()->IsArray()){
        nodes_stack_.pop_back();
        return *this;
    }
    
    throw std::logic_error("");
}

KeyItemContext Builder::Key(std::string key){
    //вызов на пустом объекте
    if (!root_.has_value()){
        throw std::logic_error("");
    }
    
    if (nodes_stack_.empty()){
        throw std::logic_error("");
    }
    
    //словарь не был начат
    if (!nodes_stack_.back()->IsDict()){
        throw std::logic_error("");
    }
    
    nodes_stack_.push_back(&std::get<Dict>(nodes_stack_.back()->GetValue())[std::move(key)]);
    
    return KeyItemContext{*this};
}

Builder& Builder::Value(Node::Value value){
    //объект уже закончен
    if (root_.has_value() && nodes_stack_.empty()){
        throw std::logic_error("");
    }
    
    Node new_node;
    
    if (std::holds_alternative<std::string>(value)){
        new_node = Node(std::get<std::string>(value));
    } else if (std::holds_alternative<std::nullptr_t>(value)) {
        new_node = Node(std::get<std::nullptr_t>(value));
    } else if (std::holds_alternative<bool>(value)){
        new_node = Node(std::get<bool>(value));
    } else if (std::holds_alternative<int>(value)){
        new_node = Node(std::get<int>(value));
    } else if (std::holds_alternative<double>(value)){
        new_node = Node(std::get<double>(value));
    } else if (std::holds_alternative<Array>(value)){
        new_node = Node(std::get<Array>(value));
    } else if (std::holds_alternative<Dict>(value)){
        new_node = Node(std::get<Dict>(value));
    }
    
    //вызов сразу после конструктора, объект завершен
    if (!root_.has_value()) {
        root_ = std::move(new_node);
    } else if (nodes_stack_.back()->IsArray()){
        std::get<Array>(nodes_stack_.back()->GetValue()).push_back(std::move(new_node));
    } else if (nodes_stack_.back()->IsNull()){
        *nodes_stack_.back() = std::move(new_node);
        
        //выбрасываю из стека элемент словаря
        nodes_stack_.pop_back();
    } else {
        throw std::logic_error("");
    }
    
    return *this;
}

Node Builder::Build(){
    if (root_.has_value() && nodes_stack_.empty()){
        return root_.value();;
    }
    
    throw std::logic_error("");
}

DictItemContext BaseContext::StartDict(){
    return builder_.StartDict();
}
ArrayItemContext BaseContext::StartArray(){
    return builder_.StartArray();
}
Builder& BaseContext::EndDict(){
    return builder_.EndDict();
}
Builder& BaseContext::EndArray(){
    return builder_.EndArray();
}
KeyItemContext BaseContext::Key(std::string key){
    return builder_.Key(key);
}
Builder& BaseContext::Value(Node::Value value){
    return builder_.Value(value);
}

DictItemContext::DictItemContext(Builder& builder): BaseContext(builder){}
ArrayItemContext::ArrayItemContext(Builder& builder): BaseContext(builder){}
KeyItemContext::KeyItemContext(Builder& builder) : BaseContext(builder) {}

}



