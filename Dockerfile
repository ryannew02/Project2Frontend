FROM ubuntu:22.04

RUN apt-get update && apt-get install -y curl g++ && \
    curl -fsSL https://deb.nodesource.com/setup_20.x | bash - && \
    apt-get install -y nodejs

WORKDIR /app

# Copy and install the Test folder's dependencies
COPY Test/package.json ./Test/
RUN cd Test && npm install

# Copy everything else
COPY . .

# Compile C++
RUN g++ -o Test/Backend/build/dataStructure Test/Backend/src/test.cpp

# Build React (from root)
RUN npm install && npm run build

EXPOSE 3000

CMD ["node", "Test/app.js"]