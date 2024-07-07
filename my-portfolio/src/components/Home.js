import React from "react";

const Home = () => (
  <section id="home" style={{ display: "flex" }}>
    <div className="text-content" style={{ flex: 1 }}>
      <h2>Hello, I'm Maulana</h2>
      <p>
        A college student who is exploring the world of programming with
        passion. Currently, I'm studying in the IT field and aspire to establish
        an inclusive programming community in Indonesia, a place where everyone
        can learn and grow together without limitations. If you are interested
        in collaborating or exchanging ideas about programming, you can contact
        me via the email page maulanazahiri31@gmail.com. Then to conclude, let
        us together create a supportive environment for growth and innovation in
        the world of programming!.
      </p>
    </div>
    <div className="profile-picture">
      <img
        src={process.env.PUBLIC_URL + "/img/lana.png"}
        alt="Maulana"
        style={{ width: "200px", height: "200px", borderRadius: "50%" }}
      />
    </div>
  </section>
);

export default Home;
