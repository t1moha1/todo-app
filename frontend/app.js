const API = '//'+location.hostname+':8080';

async function fetchTasks() {
  const res = await fetch(`${API}/tasks`);
  const tasks = await res.json();
  renderTasks(tasks);
}

function renderTasks(tasks) {
  const ul = document.getElementById('todo-list');
  ul.innerHTML = '';
  tasks.forEach(t => {
    const li = document.createElement('li');
    li.innerHTML = `
      <input type="checkbox" ${t.done ? 'checked' : ''} data-id="${t.id}">
      <span>${t.title}</span>
      <button data-id="${t.id}">✕</button>
    `;
    ul.appendChild(li);
  });

  // Обработчики
  ul.querySelectorAll('input[type=checkbox]').forEach(cb => {
    cb.addEventListener('change', async e => {
      const id = e.target.dataset.id;
      await fetch(`${API}/tasks/${id}`, {
        method: 'PUT',
        headers: {'Content-Type':'application/json'},
        body: JSON.stringify({done: e.target.checked})
      });
      fetchTasks();
    });
  });
  ul.querySelectorAll('button').forEach(btn => {
    btn.addEventListener('click', async e => {
      const id = e.target.dataset.id;
      await fetch(`${API}/tasks/${id}`, { method: 'DELETE' });
      fetchTasks();
    });
  });
}

document.getElementById('add-btn').addEventListener('click', async () => {
  const inp = document.getElementById('new-task');
  const title = inp.value.trim();
  if (!title) return;
  await fetch(`${API}/tasks`, {
    method: 'POST',
    headers: {'Content-Type':'application/json'},
    body: JSON.stringify({title})
  });
  inp.value = '';
  fetchTasks();
});

fetchTasks();