const book_manager = {
   template: `
   <div class="container" id="book-manager">
      <div class="row">
         <div class="col-12">
            <h1>{{title}}</h1>
         </div>
      </div>

      <div class="row">
         <div class="col-12">
            <h3>All Books</h3>
            <table class="table table-bordered">
               <tr class="table-success">
                  <th class="clickable" v-on:click.prevent="onSortTitle()">Title <span class="glyphicon glyphicon-triangle-bottom" aria-hidden="true"></span></th>
                  <th class="clickable" v-on:click.prevent="onSortAuthor()">Author <span class="glyphicon glyphicon-triangle-bottom" aria-hidden="true"></span></th>
                  <th class="clickable" v-on:click.prevent="onSortYear()">Year <span class="glyphicon glyphicon-triangle-bottom" aria-hidden="true"></span></th>
                  <th>Read</th>
                  <th>Rating</th>
                  <th>Update</th>
                  <th>Delete</th>
               </tr>
               <tr v-for="(b, index) in books">
                  <td>{{b.title}}</td>
                  <td>{{b.author}}</td>
                  <td>{{b.year}}</td>
                  <td v-if="b.read" class="text-success em">✓</td>
                  <td v-else> </td>
                  <td v-if="b.rating">{{b.rating}} <span class="icon-star em" aria-hidden="true">★</span></td>
                  <td v-else> </td>
                  <td v-on:click.prevent="onEdit(index)"><a class="em clickable">✎</a></td>
                  <td v-on:click.prevent="onDelete(index)"><a class="text-danger em clickable">✗</a></td>
               </tr>
            </table>
         </div>
      </div>
   </div>
   
   `,
   
      name: 'book_manager',
      data () {
         return {
            title: 'Book Manager',
            books: [],
            book: 
            {
               id: 0,
               title: '',
               year: '',
               author: '',
               read: false,
               rating: 0
            },
            errors: 
            {
               title: '',
               author: '',
               year: '',
               misc: '',
               rating: ''
            },
            selectedStars: 0
         }
      },
      
      components: {
         rate
      },
      
      beforeMount() 
      {
         let logoutItem = document.getElementById("logout-menu");
         logoutItem.style.display='block';
         let loginItem = document.getElementById("login-menu");
         loginItem.style.display='none';
         let registerItem = document.getElementById("register-menu");
         registerItem.style.display='none';
      },
      
      mounted() 
      {
         let vm = this;
         var token = localStorage.getItem("token");
         axios.get('/api/v1/books?token='+token)
            .then(function(response) {
               for(let i = 0; i < response.data.books.length; i++) {
                  let book = [];
                  book.id = response.data.books[i].id;
                  book.title = response.data.books[i].title;
                  book.author = response.data.books[i].author;
                  book.year = response.data.books[i].year;
                  book.read = response.data.books[i].read != 0;
                  book.rating = response.data.books[i].rating;
                  vm.books.push(book);
               }
               
            })
            .catch(function(error) {
         });
      },

      methods: 
      {
         onSortAuthor() 
         {
            this.books.sort(function(a,b) 
            {
               let author1 = a.author.toLowerCase();
               let author2 = b.author.toLowerCase();

               if(author1 < author2) {
                  return -1;
               }
               if(author1 > author2) {
                  return 1;
               }

               return 0;
            });
         },

         onSortTitle() 
         {
            this.books.sort(function(a,b) 
            {
               let title1 = a.title.toLowerCase();
               let title2 = b.title.toLowerCase();

               if(title1 < title2) {
                  return -1;
               }
               if(title1 > title2) {
                  return 1;
               }

               return 0;
            });
         },

         onSortYear() 
         {
            this.books.sort(function(a,b) {
               let year1 = a.year.toLowerCase();
               let year2 = b.year.toLowerCase();

               if(year1 < year2) {
                  return -1;
               }
               if(year1 > year2) {
                  return 1;
               }

               return 0;
            });
         },

         onEdit (index) 
         {
            // Create a new copy of the book object data.
            Object.assign(this.book, this.books[index]);
            router.push({path: 'edit', 
                         query: {
                            mode: 'edit',
                            book_id:     this.book.id,
                            book_title:  this.book.title, 
                            book_author: this.book.author,
                            book_year:   this.book.year,
                            book_read:   this.book.read,
                            book_rating: this.book.rating
                        }});
         },

         onDelete (index) 
         {
            let vm = this;
            var token = localStorage.getItem("token");
            axios.delete('/api/v1/books/' + vm.books[index].id+'?token='+token)
                 .then(function(response) 
                 {
                     vm.books.splice(index, 1);
                 })
                 .catch(function(error) 
                 {
                     vm.displayErrors(error);
                 });
         },

         onRatingSelected(rating) 
         {
            this.book.rating = rating;
         }
      }
  }
