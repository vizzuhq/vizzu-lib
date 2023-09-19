#ifndef INTERFACEJS_H
#define INTERFACEJS_H

extern "C" {
extern void openUrl(const char *);
extern void setCursor(const void *target, const char *cursor);
extern void callLater(void (*)(void *), void *, int later);
}

#endif