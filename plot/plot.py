import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from matplotlib.colors import Normalize

#- lecture du ficher

with open('../plot/data.txt', 'r') as file:
        lines = file.readlines()

#- stockage des points, cluster par cluster 

clusters = {}

for line in lines:
        split_line = line.split()
        point = [float(x) for x in split_line[:-1]]
        cluster_id = int(split_line[-1])
        clusters.setdefault(cluster_id, []).append(point)


#- formatage et coloration des clusters

clusters_ids = clusters.keys()
cmap = cm.rainbow # carte de couleurs, au choix
norm = Normalize(vmin=0, vmax=max(clusters_ids))

for cluster_id in clusters_ids:

        if cluster_id >= 0:
                color = cmap(norm(cluster_id))
                label = "cluster " + str(cluster_id)
        elif cluster_id == -1: # point isolé
                color = "white"
                label = "point isolé"
        else: # point non défini
                color = "black"
                label = "point non défini"

        clusters[cluster_id] = (np.stack(clusters[cluster_id], 0), color, label)

#- Dessin 

fig, ax = plt.subplots(figsize=(6,6))
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_title('Exemple de clustering')

for cluster_id, (points, color, label) in clusters.items():
        ax.scatter(points[:,0], points[:,1], color=color, label=label, edgecolor="black")

# légende en ordre alphabétique, optionnel mais plus joli
handles, labels = ax.get_legend_handles_labels() 
handles, labels = zip(* sorted(zip(handles, labels), key=lambda t : t[1]))
ax.legend(handles, labels)

plt.show()