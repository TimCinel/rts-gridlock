int main(void) {
    //AbstractController *mock = new AbstractController();
    char *name = "/i1";
    mq_Message* mes;

    
    SendMessage(name, mes);
    sleep(1);
    Queue q(name);
    sleep(1);
    SendMessage(name, mes);
    sleep(1);
}
