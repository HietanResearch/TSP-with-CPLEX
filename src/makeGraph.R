args <- commandArgs(trailingOnly=TRUE)
nodes_filename <- args[1]
routes_filename <- args[2]
output_filename <- args[3]

library(igraph)

nodes <- as.matrix(read.csv(nodes_filename, header=F))
routes <- read.csv(routes_filename, header=F)

edges_arg <- c()

for(y in 1:nrow(routes)){
	for(x in 1:ncol(routes)){
		if(routes[y, x] == 1){
			edges_arg <- append(edges_arg, c(y, x))
		}
	}
}

g <- graph(edges=edges_arg)

shape_arg <- c("square", rep("circle", times=(vcount(g))))

pdf(output_filename)

plot(g,
	 layout=nodes,
	 vertex.shape=shape_arg,
	 vertex.color="#E4E9D1",
	 vertex.size=10,
	 vertex.label.color="#333333",
	 edge.color="#333333",
	 edge.width=0.5,
	 edge.arrow.size=0.4
)
box()
