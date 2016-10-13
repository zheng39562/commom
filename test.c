
	static void
	conn_writecb(struct bufferevent *bev, void *user_data)
	{
		string s = "hello world (write back)";
		DEBUG_D("缓存可以接受数据，开始写数据");
		bufferevent_write(bev, s.c_str(), s.size());
		sleep(3);
	}

	static void
	conn_readcb(struct bufferevent *bev, void *user_data)
	{
		DEBUG_D("链路上有数据发送到端，正在接收数据。");

		evbuffer* output = bufferevent_get_input(bev);

		int READ_BUFFER_SIZE = 4000;
		char *m_s_pRecvBuffer = new char[READ_BUFFER_SIZE];
		memset(m_s_pRecvBuffer, 0, READ_BUFFER_SIZE);
		size_t recvSize = bufferevent_read(bev, m_s_pRecvBuffer, READ_BUFFER_SIZE);
		if(recvSize > 0){
			DEBUG_D("接受数据：" << m_s_pRecvBuffer);
		}
		sleep(3);
	}

	static void
	conn_eventcb(struct bufferevent *bev, short events, void *user_data)
	{
		DEBUG_D("错误回调");
		if (events & BEV_EVENT_EOF) {
			DEBUG_D("Connection closed.\n");
		} else if (events & BEV_EVENT_ERROR) {
			DEBUG_D("Got an error on the connection: " << strerror(errno));
		}
		bufferevent_free(bev);
	}

	static void
	listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
		struct sockaddr *sa, int socklen, void *user_data)
	{
		struct event_base *base = (event_base*)user_data;
		struct bufferevent *bev;

		bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
		if (!bev) {
			fprintf(stderr, "Error constructing bufferevent!");
			event_base_loopbreak(base);
			return;
		}
		bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
		bufferevent_enable(bev, EV_WRITE | EV_READ);
	}

	static void
	signal_cb(evutil_socket_t sig, short events, void *user_data)
	{
		struct event_base *base = (event_base*)user_data;
		struct timeval delay = { 2, 0 };

		printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

		event_base_loopexit(base, &delay);
	}

	void libevent_test(const std::string &arg){
			struct event_base *base;
			struct evconnlistener *listener;
			struct event *signal_event;

			struct sockaddr_in sin;

			base = event_base_new();
			if (!base) {
				fprintf(stderr, "Could not initialize libevent!\n");
				return;
			}

			memset(&sin, 0, sizeof(sin));
			sin.sin_family = AF_INET;
			inet_pton(AF_INET, TEST_IP.c_str(), &sin.sin_addr);
			sin.sin_port = htons(TEST_CONNECT_PORT);

		if(arg == ARG_CLIENT){
			int m_ConnectSocket = socket(PF_INET, SOCK_STREAM, 0);
			if(connect(m_ConnectSocket, (struct sockaddr *) &sin, sizeof(sin)) != 0){
				DEBUG_D("connect 失败");
			}

			struct bufferevent* bev;
			bev = bufferevent_socket_new(base, m_ConnectSocket, BEV_OPT_CLOSE_ON_FREE);

			bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
			bufferevent_enable(bev, EV_WRITE | EV_READ);

			event_base_dispatch(base);

			DEBUG_D("after event_base_dispatch");
		}
		if(arg == ARG_SERVER){
			listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
				LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
				(struct sockaddr*)&sin,
				sizeof(sin));

			if (!listener) {
				fprintf(stderr, "Could not create a listener!\n");
				return;
			}

			signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);

			if (!signal_event || event_add(signal_event, NULL)<0) {
				fprintf(stderr, "Could not create/add a signal event!\n");
				return;
			}

			event_base_dispatch(base);

			DEBUG_D("after event_base_dispatch");

			evconnlistener_free(listener);
			event_free(signal_event);
			event_base_free(base);

			printf("done\n");
			return;
		}
	}

