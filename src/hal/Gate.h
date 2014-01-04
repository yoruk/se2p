#ifndef GATE_H_
#define GATE_H_

class Gate {
public:
	static Gate* getInstance();
    virtual ~Gate();
    int status();
    void close();
    void open();

private:
    Gate();
};

#endif /* GATE_H_ */
