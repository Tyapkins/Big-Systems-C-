#pragma once

#include <memory>
#include <iostream>

// добавлено наследование от класса enable_shared_from_this
// (необходимость описана ниже у методов replaceLeft и replaceRight

namespace bintree {
    template <typename T>
struct TNode : public std::enable_shared_from_this<TNode<T>> {
        using TNodePtr = std::shared_ptr<TNode<T>>;
        using TNodeConstPtr = std::shared_ptr<const TNode<T>>;

        bool hasLeft() const {
            return bool(left);
        }

        bool hasRight() const {
            return bool(right);
        }

        bool hasParent() const {
            return bool(parent);
        }

        T& getValue() {
            return value;
        }

        const T& getValue() const {
            return value;
        }

        TNodePtr getLeft() {
            return left;
        }

        TNodeConstPtr getLeft() const {
            return left;
        }

        TNodePtr getRight() {
            return right;
        }

        TNodeConstPtr getRight() const {
            return right;
        }

        TNodePtr getParent() {
            return parent;
        }

        TNodeConstPtr getParent() const {
            return parent;
        }

        // Статический метод имеет доступ к полям класса, в том числе и к приватному конструктору
        // При передаче аргументов для создания объекта std::make_shared будет произведена попытка
        // создать объект этого класса. Однако эта попытка будет предпринята вне класса,
        // поэтому компилятор выдаст ошибку. Для избежания подобного поведения достаточно создать
        // объект явно внутри класса, к которому уже можно применить std::make_shared
        // (ошибка в методах createLeaf и fork)

        static TNodePtr createLeaf(T v) {
            auto obj = std::make_shared<TNode>(TNode(v));
            return obj;
        }

        static TNodePtr fork(T v, TNode* left, TNode* right) {
            TNodePtr ptr = std::make_shared<TNode>(TNode(v, left, right));
            setParent(ptr->getLeft(), ptr);
            setParent(ptr->getRight(), ptr);
            return ptr;
        }

        // Методам replaceLeft и replaceRight необходимо использовать shared_ptr от this.
        // Однако создание shared_ptr от raw pointer-а приведёт к тому, что деструктор будет
        // вызван дважды (double free). Для избежания подобной ситуации используется особый
        // метод у класса enable_shared_from_this из стандартной библиотеки,
        // позволяющий получить shared_ptr от this.
        // (подобная ошибка в replaceLeft и replaceRight)

        TNodePtr replaceLeft(TNodePtr l) {
            setParent(l, std::enable_shared_from_this<TNode<T>>::shared_from_this());
            setParent(left, nullptr);
            std::swap(left, l);
            return l;
        }

        TNodePtr replaceRight(TNodePtr r) {
            setParent(r, std::enable_shared_from_this<TNode<T>>::shared_from_this());
            setParent(right, nullptr);
            std::swap(r, right);
            return r;
        }

        TNodePtr replaceRightWithLeaf(T v) {
            return replaceRight(createLeaf(v));
        }

        TNodePtr replaceLeftWithLeaf(T v) {
            return replaceLeft(createLeaf(v));
        }

        TNodePtr removeLeft() {
            return replaceLeft(nullptr);
        }
        TNodePtr removeRight() {
            return replaceRight(nullptr);
        }

    private:
        T value;
        TNodePtr left = nullptr;
        TNodePtr right = nullptr;
        TNodePtr parent = nullptr;

        TNode(T v)
            : value(v)
        {
        }
        TNode(T v, TNode* left, TNode* right)
            : value(v)
            , left(left)
            , right(right)
        {
        }

        static void setParent(TNodePtr node, TNodePtr parent) {
            if (node != nullptr)
                node->parent = parent;
        }
    };
}
