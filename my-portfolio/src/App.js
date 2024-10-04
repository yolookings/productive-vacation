import React from "react";
import "./App.css";
import Header from "./components/Header";
import Home from "./components/Home";
import About from "./components/About";
import Projects from "./components/Projects";
import Contact from "./components/Contact";

function App() {
  return (
    <div className="App">
      <Header />
      <main>
        <Home />
        <About />
        <Projects />
        <Contact />
      </main>
      <footer>
        <p>&copy; 2024 Maulana Ahmad Zahiri. All rights reserved.</p>
      </footer>
    </div>
  );
}

export default App;
