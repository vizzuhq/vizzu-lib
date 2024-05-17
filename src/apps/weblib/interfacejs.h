#ifndef INTERFACEJS_H
#define INTERFACEJS_H

extern "C" {
extern void chart_openUrl(const void *, const char *);
extern void chart_doChange(const void *);
extern void
textBoundary(const char *, const char *, double *, double *);
}

#endif