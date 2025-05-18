let prioridad = 1;

function togglePrioridades() {
  const opciones = document.getElementById("prioridad-opciones");
  opciones.style.display = opciones.style.display === "flex" ? "none" : "flex";
}

document.querySelectorAll(".prioridad-option").forEach(el => {
  el.onclick = () => {
    prioridad = parseInt(el.dataset.prioridad);
    document.getElementById("prioridad-activa").innerText = prioridad;
    document.querySelector(".prioridad-seleccionada").style.backgroundColor =
      getComputedStyle(el).backgroundColor;
    document.getElementById("prioridad-opciones").style.display = "none";
  };
});

// Función para agregar un mensaje
async function agregar() {
  const texto = document.getElementById("texto").value;
  
  // removes whitespace
  if (!texto.trim()) return;

  await fetch("/mensajes", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ texto, prioridad })
  });

  document.getElementById("texto").value = "";
  cargar(); // Asume que ya existe esta función
}


// Función para cargar los mensajes desde el servidor
async function cargar() {
  const res = await fetch("/mensajes");
  const mensajes = await res.json();
  const lista = document.getElementById("lista");
  lista.innerHTML = ""; // Limpiar la lista de mensajes

    mensajes.forEach((m, i) => {
        if (m && typeof m.texto === "string") {
          const li = document.createElement("li");
          li.classList.add("mensaje", `prioridad-${m.prioridad || 1}`);

          const contenido = document.createElement("div");
          contenido.classList.add("mensaje-texto");
          contenido.textContent = m.texto;
          
          const btn = document.createElement("button");
          btn.classList.add("btn-eliminar");
          btn.textContent = "Eliminar";
          btn.onclick = async () => {
            await fetch(`/mensajes?id=${i}`, { method: "DELETE" });
            cargar();
          };

          li.appendChild(contenido);
          li.appendChild(btn);
          lista.appendChild(li);
        }
    });
}

// Cargar los mensajes cuando se abre la página
window.onload = cargar;
