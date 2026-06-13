FROM ubuntu:22.04

# Install curl and Node.js 20 (modern enough for Vite)
RUN apt-get update && apt-get install -y curl g++ && \
    curl -fsSL https://deb.nodesource.com/setup_20.x | bash - && \
    apt-get install -y nodejs

WORKDIR /app

COPY package.json .
RUN npm install

COPY . .

RUN g++ -o Test/Backend/build/dataStructure Test/Backend/src/test.cpp

RUN npm run build

EXPOSE 3000

CMD ["node", "Test/app.js"]