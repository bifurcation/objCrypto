FROM alpine as base
LABEL description="Tools to build ObjCrypto Library "

# Build tools
RUN apk add build-base
RUN apk add cmake
RUN apk add tcsh
RUN mkdir /src
WORKDIR /src
CMD /bin/tcsh
