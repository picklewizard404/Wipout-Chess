#ifndef NODE_INVENTED
template<class P>
class Node {
public:
    P item;
    Node* previous;
    Node* next;
    Node(P my_item);
    //P my_item();
};
template<class P> Node<P>::Node(P my_item) {
    item = my_item;
}
#endif //NODE_INVENTED