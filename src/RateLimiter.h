#ifndef _RATELIMITER_H_
#define _RATELIMITER_H_

class RateLimiter {
public:
    RateLimiter(long everyMillis);
    bool due();
    void reset();

private:
    long everyMillis;
    long lastMillis;
};

#endif
