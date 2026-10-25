#include<stdio.h>
#include<stdlib.h>
struct ListNode {
     int val;
     struct ListNode *next;
 };
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode dummy;
    dummy.val=0;
    dummy.next=NULL;
    struct ListNode*tail=&dummy;
    int jw=0;
    while(l1!=NULL||l2!=NULL||jw!=0){
        int a = (l1 != NULL) ? l1->val : 0;
        int b = (l2 != NULL) ? l2->val : 0;
        int sum=a+b+jw;
        jw=sum/10;
        struct ListNode*node=malloc(sizeof(struct ListNode));
        node->val=sum%10;
        node->next=NULL;
        tail->next=node;
        tail=node;
        if (l1 != NULL) l1 = l1->next;  
        if (l2 != NULL) l2 = l2->next;
    }
    return dummy.next;
}
      