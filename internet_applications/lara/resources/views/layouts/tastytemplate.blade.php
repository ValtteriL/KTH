<!DOCTYPE html>
<html lang="en">
    <head>
        @include('includes.head')
        @yield('title')
        @yield('additional_head')
    </head>
    <body>
        <!-- Header -->
            @include('includes.header')
        <!-- Middle section -->
            @yield('middle_section')
        <!-- Footer -->
            @include('includes.footer')
    </body>
</html>
