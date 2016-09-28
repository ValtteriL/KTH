        <!-- Header -->
        <header>
            <h1>Tasty Recipes</h1>
            <p>Welcome to the tastiest site you'll ever visit</p>
        </header>

        <!-- Menu -->
        <label for="show-menu" class="show-menu">Show Menu</label>
        <input type="checkbox" id="show-menu" role="button">
        <div id="menu">
            <ul>
                <li><a {{ Request::is('/') ? 'class=active' : '' }} href="/">Home</a></li>
                <li><a {{ Request::is('calendar') ? 'class=active' : '' }} href="/calendar">Calendar</a></li>
                <li class="dropdown">
                    <a href="#" class="dropbtn">Recipes</a>
                    <div class="dropdown-content">
                        <a {{ Request::is('recipes/meatballs') ? 'class=active' : '' }} href="/recipes/meatballs">Meatballs</a>
                        <a {{ Request::is('recipes/pancakes') ? 'class=active' : '' }} href="/recipes/pancakes">Pancakes</a>
                    </div>
                </li>

                @if (Auth::check()) 
                <li class="login"><a href="#">Logged in as {{ Auth::user()->name }}</a></li>
                <li class="login"><a href="{{ url('/logout') }}" onclick="event.preventDefault();
                                                 document.getElementById('logout-form').submit();">
                                        Logout
                                  </a></li>
                <form id="logout-form" action="{{ url('/logout') }}" method="POST" style="display: none;">
                    {{ csrf_field() }}
                </form>

                @else 
                <li class="login"><a {{ Request::is('login') ? 'class=active' : '' }} href="/login">Log in</a></li>
                <li class="login"><a {{ Request::is('register') ? 'class=active' : '' }} href="/register">Register</a></li>
                @endif 

            </ul>
        </div>

