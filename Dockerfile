# Use Ubuntu with Node and GCC available
FROM ubuntu:22.04

# Install Node.js, npm, and g++
RUN apt-get update && apt-get install -y \
    nodejs \
    npm \
    g++

# Set working directory
WORKDIR /app

# Copy package.json and install Node dependencies
COPY package.json .
RUN npm install

# Copy everything else
COPY . .

# Compile the C++ source into a Linux binary (no .exe on Linux)
RUN g++ -o Test/Backend/build/dataStructure Test/Backend/src/test.cpp

# Build the React app into static files
RUN npm run build

# Expose the Express port
EXPOSE 3000

# Start the Express server
CMD ["node", "app.js"]