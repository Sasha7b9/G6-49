#include "defines.h"
#include "common/Interface_p.h"
#include "Display/Text.h"
#include "Utils/List.h"


template void               List<Task>::Append(Task *);
template bool               List<Task>::IsMember(Task *);
template ListElement<Task> *List<Task>::First();
template                    List<Task>::List();
template void               List<Task>::Remove(Task *);
template uint               List<Task>::Size() const;

template                    ListElement<Task>::~ListElement();
template Task              *ListElement<Task>::Get();
template ListElement<Task> *ListElement<Task>::Next();

template                    List<String>::List();
template void               List<String>::Clear();
template void               List<String>::Append(String *);
template void               List<String>::Remove(String *);



template <class T>
List<T>::List() : head(nullptr)
{

}


template <class T>
void List<T>::Append(T *t)
{
    ListElement<T> *element = new ListElement<T>(t); //-V2511

    ListElement<T> *last = Last();

    if (last == nullptr)
    {
        head = element;
    }
    else
    {
        last->next = element;
        element->prev = last;
    }
}


template <class T>
void List<T>::Remove(T *removed) //-V2506
{
    ListElement<T> *element = First();

    while (element)
    {
        if (element->Get() == removed)
        {
            // ����� ���� �������

            ListElement<T> *next = element->next;
            ListElement<T> *prev = element->prev;

            if (element == First())             // ���� ��������� ������� - ������ � ������
            {
                if (next)
                {
                    next->prev = nullptr;       // �� ������ ������ ��������� �������
                }
                head = next;                    // � ���������� ��� � ������
            }
            else if (element == Last())         // ���� ��������� ������� - ��������� � ������
            {
                prev->next = nullptr;           // �� ������ ��������� � ���������� ������� ������� �����
            }
            else
            {
                next->prev = prev;
                prev->next = next;
            }

            return;
        }

        element = element->Next();
    }
}


template <class T>
ListElement<T> *List<T>::Last() //-V2506
{
    if (head == nullptr)
    {
        return nullptr;
    }

    ListElement<T> *element = First();

    while (element->next != nullptr)
    {
        element = element->Next();
    }

    return element;
}


template <class T>
bool List<T>::IsMember(T *t) //-V2506
{
    ListElement<T> *element = First();

    while (element != nullptr)
    {
        if (element->Get()->Equals(element->Get(), t))
        {
            return true;
        }

        element = element->Next();
    }

    return false;
}


template <class T>
ListElement<T> *List<T>::First()
{
    return head;
}


template <class T>
T *ListElement<T>::Get()
{
    return value;
}


template <class T>
ListElement<T> *ListElement<T>::Next()
{
    return next;
}


template <class T>
ListElement<T>::~ListElement()
{
    delete value; //-V2511
}


template <class T>
uint List<T>::Size() const
{
    uint size = 0;

    ListElement<T> *element = head;

    while (element)
    {
        size++;
        element = element->Next();
    }

    return size;
}


template <class T>
void List<T>::Clear()
{
    while (Size() != 0U)
    {
        T *elem = First()->Get();
        Remove(elem);
        delete elem; //-V2511
    }
}

